//
//  Controller.swift
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 2/24/24.
//

import Foundation
import AppKit

// TODO: Currently Serial() manages the opened/closed state. It makes more sense to control the serial port from this controller
// using something like this: http://codeworkshop.net/posts/power-nap-and-the-network

class Controller {
	
	let serial: Serial
	var timer: Timer?
	let collector = Collector()
	var bootTime: __darwin_time_t
	
	init() {
		// capture the system boot time for uptime calculations
		let name = "kern.boottime"
		var size = MemoryLayout<timeval>.size
		var timeval = timeval(tv_sec: 0, tv_usec: 0)
		if sysctlbyname(name, &timeval, &size, nil, 0) != -1 {
			debugLog("timeval seconds = \(timeval.tv_sec)")
			bootTime = timeval.tv_sec
		}
		else {
			bootTime = 0
		}
		
		let bsdPath: String
		if let defaultBsdPath = Serial.defaultBsdPath {
			bsdPath = defaultBsdPath
		}
		else {
			print("No path to serial device")
			bsdPath = ""
			//exit(1)
		}

		debugLog("bsdPath = '\(bsdPath)'")
		serial = Serial(bsdPath: bsdPath)
		if !serial.open() {
			print("could not open serial device = '\(bsdPath)'")
			//exit(1)
		}
		print("connected to serial device = '\(bsdPath)'")
		
		NotificationCenter.default.addObserver(self, selector: #selector(serialStateDidChange), name: NSNotification.Name.SerialStateDidChange, object: serial)
		NSWorkspace.shared.notificationCenter.addObserver(self, selector: #selector(willSleep), name: NSWorkspace.willSleepNotification, object: nil)
		NSWorkspace.shared.notificationCenter.addObserver(self, selector: #selector(didWake), name: NSWorkspace.didWakeNotification, object: nil)
		//		NSWorkspace.shared.notificationCenter.addObserver(self, selector: #selector(willSleep), name: NSWorkspace.screensDidSleepNotification, object: nil)
		//		NSWorkspace.shared.notificationCenter.addObserver(self, selector: #selector(didWake), name: NSWorkspace.screensDidWakeNotification, object: nil)
		NSWorkspace.shared.notificationCenter.addObserver(self, selector: #selector(didBecomeActive), name: NSWorkspace.sessionDidBecomeActiveNotification, object: nil)
		NSWorkspace.shared.notificationCenter.addObserver(self, selector: #selector(didResignActive), name: NSWorkspace.sessionDidResignActiveNotification, object: nil)
	}
	
	deinit {
		serial.close()
	}
	
	@objc
	func serialStateDidChange(_ notification: Notification) {
		print("***** serial state changed")
		if !serial.isOpen {
			print("lost connection to serial device = '\(serial.bsdPath!)'")
			exit(1)
		}
	}
	
	@objc
	func willSleep(_ notification: Notification) {
		print("***** will sleep")
		serial.close()
	}
	
	@objc
	func didWake(_ notification: Notification) {
		print("***** did wake")
		serial.open()
	}
	
	@objc
	func didBecomeActive(_ notification: Notification) {
		print("***** did become active")
	}
	
	@objc
	func didResignActive(_ notification: Notification) {
		print("***** did resign active")
	}

#if false
	func start() {
		while true {
			autoreleasepool {
				collectAndTransfer()
				sleep(1)
			}
		}
	}
#else
#if true
	@objc
	func update() {
		autoreleasepool {
			collectAndTransfer()
		}
	}
	
	func start() {
		guard timer == nil else { return }
		
		timer = Timer.scheduledTimer(timeInterval: 1.0, target: self, selector: #selector(update), userInfo: nil, repeats: true)
	}
#else
	func start() {
		guard timer == nil else { return }

		timer = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { timer in
			autoreleasepool {
				collectAndTransfer()
			}
		}
	}
#endif
#endif
	
	func stop() {
		guard let timer else { return }
		
		timer.invalidate()
		self.timer = nil
	}
	
	private func collectAndTransfer() {
		self.collector.collect()
		
		return
#warning("do this check before building Transfer struct")
		//if self.serial.isOpen {
		do {
			var transfer = Transfer()
			if let processorSample = self.collector.processorSamples.last {
				let totalUsage = processorSample.coreSamples.reduce(0, { partialResult, coreSample in
					partialResult + coreSample.usage
				})
				transfer.cpuUsage = (Float(totalUsage) / Float(processorSample.coreSamples.count))
			}
			if let networkSample = self.collector.networkSamples.last {
				transfer.networkReceivedPackets = networkSample.receivedPackets
				transfer.networkReceivedBps = networkSample.receivedBitsPerSecond
				transfer.networkSentPackets = networkSample.sentPackets
				transfer.networkSentBps = networkSample.sentBitsPerSecond
			}
			if let diskSample = self.collector.diskSamples.last {
				transfer.diskReadCount = diskSample.readCount
				transfer.diskReadBytes = diskSample.readBytes
				transfer.diskWriteCount = diskSample.writeCount
				transfer.diskWriteBytes = diskSample.writeBytes
			}
			if let memorySample = self.collector.memorySamples.last {
				transfer.memoryWiredSize = memorySample.wiredSize
				transfer.memoryAppSize = memorySample.appMemorySize
				transfer.memoryCompressedSize = memorySample.compressedSize
			}
			if let loadSample = self.collector.loadSamples.last {
				transfer.loadOneMinute = Float(loadSample.oneMinute)
				transfer.loadFiveMinutes = Float(loadSample.fiveMinutes)
				transfer.loadFifteenMinutes = Float(loadSample.fifteenMinutes)
			}
			
			let now = time(nil)
			transfer.uptime = now - bootTime
			
			let encoder = JSONEncoder()
			encoder.keyEncodingStrategy = .convertToSnakeCase
			encoder.outputFormatting = [.withoutEscapingSlashes, .prettyPrinted]
			//encoder.outputFormatting = [.withoutEscapingSlashes]
			do {
				let data = try encoder.encode(transfer)
				if let message = String(data: data, encoding: .utf8) {
#warning("do this check before building Transfer struct")
					if self.serial.isOpen {
						self.serial.send(message)
					}
					else {
						debugLog("message = \(message)")
					}
					if let debug = self.serial.read() {
						// print any debugging output generated by the Arduino
						print(debug)
					}
					
				}
			}
			catch {
				debugLog("error = \(error.localizedDescription)")
			}
		}
	}

}
