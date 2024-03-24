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
	}
	
	deinit {
		serial.close()
	}

	// MARK: - Notifications
	
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
	
	// MARK: - Public
	
	func start() {
		guard timer == nil else { return }

		timer = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { timer in
			autoreleasepool {
				self.collectAndTransfer()
			}
		}
	}
	
	func stop() {
		guard let timer else { return }
		
		timer.invalidate()
		self.timer = nil
	}
	
	// MARK: - Private

	private func collectAndTransfer() {
		self.collector.collect()

#if DEBUG
		let doTransfer = true
#else
		let doTransfer = self.serial.isOpen
#endif
		
		if doTransfer {
			var transfer = Transfer()
			if let processorSample = self.collector.processorSamples.last {
				let totalUsage = processorSample.coreSamples.reduce(0, { partialResult, coreSample in
					partialResult + coreSample.usage
				})
				transfer.cpuUsage = (Float(totalUsage) / Float(processorSample.coreSamples.count))
			}
			if let graphicsSample = collector.graphicsSamples.last {
				transfer.gpuUsage = Float(graphicsSample.deviceUtilization)
				transfer.gpuSize = graphicsSample.memorySize
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
				transfer.memoryPhysicalSize = ProcessInfo.processInfo.physicalMemory
				transfer.memoryWiredSize = memorySample.wiredSize
				transfer.memoryAppSize = memorySample.appMemorySize
				transfer.memoryCompressedSize = memorySample.compressedSize
			}
			if let loadSample = self.collector.loadSamples.last {
				transfer.loadOneMinute = Float(loadSample.oneMinute)
				transfer.loadFiveMinutes = Float(loadSample.fiveMinutes)
				transfer.loadFifteenMinutes = Float(loadSample.fifteenMinutes)
			}
			if let storageSample = self.collector.storageSamples.last {
				var transferVolumes: [TransferVolume] = []
				for volumeSample in storageSample.volumeSamples {
					let transferVolume = TransferVolume(name: volumeSample.name, totalBytes: volumeSample.totalSize, availableBytes: volumeSample.availableSize)
					transferVolumes.append(transferVolume)
				}
				transfer.volumes = transferVolumes
			}
			
			let now = time(nil)
			transfer.uptime = now - bootTime
			
			let encoder = JSONEncoder()
			encoder.keyEncodingStrategy = .convertToSnakeCase
#if DEBUG && true
			encoder.outputFormatting = [.withoutEscapingSlashes, .prettyPrinted]
#else
			encoder.outputFormatting = [.withoutEscapingSlashes]
#endif
			do {
				let data = try encoder.encode(transfer)
				if let message = String(data: data, encoding: .utf8) {
					if self.serial.isOpen {
						self.serial.send(message)
					}
#if DEBUG && false
					print("message = \(message)")
#endif
					if let debug = self.serial.read() {
#if DEBUG && true
						// print any debugging output generated by the Arduino
						print(debug)
#endif
					}
					
				}
			}
			catch {
				debugLog("error = \(error.localizedDescription)")
			}
		}
	}

}
