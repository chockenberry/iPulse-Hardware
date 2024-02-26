//
//  Controller.swift
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 2/24/24.
//

import Foundation

class Controller {
	
	let serial: Serial
	var timer: Timer?
	let collector = Collector()
	
	init() {
		serial = Serial(bsdPath: "/dev/cu.usbmodem1444201")
		serial.open()
	}

	deinit {
		serial.close()
	}
	
	func start() {
		guard timer == nil else { return }
		timer = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { timer in
			self.collector.collect()
			if let processorSample = self.collector.processorSamples.last {
				let totalUsage = processorSample.coreSamples.reduce(0, { partialResult, coreSample in
					partialResult + coreSample.usage
				})
				let usage = totalUsage / Double(processorSample.coreSamples.count)
				let message = "\(usage)"
				self.serial.send(message)
			}
		}
	}
	
	func stop() {
		guard let timer else { return }
		
		timer.invalidate()
		self.timer = nil
	}
	
}
