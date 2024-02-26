//
//  Collector.swift
//  iPulseMobile
//
//  Created by Craig Hockenberry on 12/14/23.
//

import Foundation
import Network
import os.log
import Metal

extension FormatStyle where Self == FloatingPointFormatStyle<Double> {
	static var normalized: FloatingPointFormatStyle<Double> {
		let style = FloatingPointFormatStyle<Double>()
		return style.scale(100).precision(.fractionLength(1))
	}
	static var fractionless: FloatingPointFormatStyle<Double> {
		let style = FloatingPointFormatStyle<Double>()
		return style.scale(100).precision(.fractionLength(0))
	}
}

extension Double {
	
	func bytes() -> String {
		if self == 0.0 {
			return "–"
		}
#if true
		let maxScale = Int(floor(log2(self)))
		
		let suffix: String
		let scalingPower: Double
		let decimalPlaces: Int
		switch maxScale {
		case 0..<10: // 0 .. 1K
			suffix = "bytes"
			scalingPower = 0
			decimalPlaces = 0
		case 10..<20: // 1K .. 1MB
			suffix = "KB"
			scalingPower = 10
			decimalPlaces = 0
		case 20..<30: // 1MB .. 1GB
			suffix = "MB"
			scalingPower = 20
			decimalPlaces = 0
		default: // over 1GB
			suffix = "GB"
			scalingPower = 30
			decimalPlaces = 1
		}
		let scaling = pow(2, scalingPower)
		return String(format: "%.*f %@", decimalPlaces, self / scaling, suffix)
		
#else
		let measurement = Measurement(value: self, unit: UnitInformationStorage.bytes)
		
#if false
		var formattedMeasurement = measurement.formatted(.byteCount(style: .memory, allowedUnits: .all, spellsOutZero: false, includesActualByteCount: false))
		if let suffix {
			formattedMeasurement += suffix
		}
		return formattedMeasurement
#else
		let unitTypes = [ UnitInformationStorage.pebibytes, UnitInformationStorage.gibibytes, UnitInformationStorage.mebibytes, UnitInformationStorage.kibibytes ]
		//		let unitTypes = [ UnitInformationStorage.petabytes, UnitInformationStorage.gigabytes, UnitInformationStorage.megabytes, UnitInformationStorage.kilobytes ]
		var convertedMeasurement = Measurement(value: 0, unit: UnitInformationStorage.bytes)
		for unitType in unitTypes {
			convertedMeasurement = measurement.converted(to: unitType)
			if convertedMeasurement.value >= 1 {
				break
			}
		}
		
		let measurementFormatter = MeasurementFormatter()
		measurementFormatter.unitStyle = .medium
		measurementFormatter.unitOptions = .providedUnit
		measurementFormatter.numberFormatter.maximumFractionDigits = fractionDigits
		measurementFormatter.numberFormatter.minimumFractionDigits = fractionDigits
		
		var formattedMeasurement = measurementFormatter.string(from: convertedMeasurement)
		if let suffix {
			formattedMeasurement += suffix
		}
		return formattedMeasurement
#endif
#endif
	}
	
	func bits() -> String {
#if true
		if self == 0.0 {
			return "–"
		}

		let maxScale = Int(floor(log10(self)))
		
		let bitsPerSecond = self * 8.0
		let suffix: String
		let scalingPower: CGFloat
		let decimalPlaces: Int
		switch maxScale {
		case 0..<2: // 0 .. 100
			suffix = "bps"
			scalingPower = 0
			decimalPlaces = 0
		case 3..<6: // 1,000 .. 1,000,000
			suffix = "Kbps"
			scalingPower = 3
			decimalPlaces = 0
		case 6..<9: // 1,000,000 .. 1,000,000,000
			suffix = "Mbps"
			scalingPower = 6
			decimalPlaces = 1
		default: // over 1,000,000,000
			suffix = "Gbps"
			scalingPower = 9
			decimalPlaces = 1
		}
		let scaling = pow(10, scalingPower)
		return String(format: "%.*f %@", decimalPlaces, bitsPerSecond / scaling, suffix)
		
#else
		let measurement = Measurement(value: self, unit: UnitInformationStorage.bytes)
		
		let unitTypes = [ UnitInformationStorage.petabits, UnitInformationStorage.gigabits, UnitInformationStorage.megabits, UnitInformationStorage.kilobits ]
		//		let unitTypes = [ UnitInformationStorage.pebibits, UnitInformationStorage.gibibits, UnitInformationStorage.mebibits, UnitInformationStorage.kibibits ]
		var convertedMeasurement = Measurement(value: 0, unit: UnitInformationStorage.bytes)
		for unitType in unitTypes {
			convertedMeasurement = measurement.converted(to: unitType)
			if convertedMeasurement.value >= 1 {
				break
			}
		}
		
		let measurementFormatter = MeasurementFormatter()
		measurementFormatter.unitStyle = .medium
		measurementFormatter.unitOptions = .providedUnit
		measurementFormatter.numberFormatter.maximumFractionDigits = 1
		measurementFormatter.numberFormatter.minimumFractionDigits = 1
		
		var formattedMeasurement = measurementFormatter.string(from: convertedMeasurement)
		if let suffix {
			formattedMeasurement += suffix
		}
		return formattedMeasurement
#endif
	}
	
}

// MARK: -

// memory
//		free page count
//		active page count
//		inactive page count
//		wired page count
//		compressed page count
//
//		page in delta
//		page out delta
//
// https://stackoverflow.com/a/35833857/132867 - good explainer on paging

struct MemorySample {
	let wiredPageCount: UInt32
	let activePageCount: UInt32
	let inactivePageCount: UInt32
	let freePageCount: UInt32
	let compressedPageCount: UInt32
	let internalPageCount: UInt32
	let externalPageCount: UInt32
	let purgablePageCount: UInt32
	
	let pageInCount: UInt32
	let pageOutCount: UInt32
	
	var totalPageCount: UInt32 {
		wiredPageCount + activePageCount + inactivePageCount + freePageCount + compressedPageCount
		//memoryUsedCount + cachedFilesCount + freePageCount
	}

	// https://support.apple.com/guide/activity-monitor/view-memory-usage-actmntr1004/mac
	// https://apple.stackexchange.com/questions/258166/what-is-app-memory-calculated-from
	// https://stackoverflow.com/questions/31469355/how-to-calculate-app-and-cache-memory-like-activity-monitor-in-objective-c-in-ma
	
	var appMemoryCount: UInt32 {
		internalPageCount - purgablePageCount
	}

	var cachedFilesCount: UInt32 {
		externalPageCount + purgablePageCount
	}

	var memoryUsedCount: UInt32 {
		wiredPageCount + appMemoryCount + inactivePageCount + compressedPageCount - cachedFilesCount
	}

	var pageSize: UInt32 = {
		var pageSize = vm_size_t()
		guard host_page_size(mach_host_self(), &pageSize) == KERN_SUCCESS else { return 0 }
		return UInt32(pageSize)
	}()

	var totalSize: UInt64 {
		return UInt64(totalPageCount) * UInt64(pageSize)
	}

	var wiredSize: UInt64 {
		return UInt64(wiredPageCount) * UInt64(pageSize)
	}
	
	var activeSize: UInt64 {
		return UInt64(activePageCount) * UInt64(pageSize)
	}
	
	var inactiveSize: UInt64 {
		return UInt64(inactivePageCount) * UInt64(pageSize)
	}
	
	var freeSize: UInt64 {
		return UInt64(freePageCount) * UInt64(pageSize)
	}
	
	var compressedSize: UInt64 {
		return UInt64(compressedPageCount) * UInt64(pageSize)
	}

	var appMemorySize: UInt64 {
		return UInt64(appMemoryCount) * UInt64(pageSize)
	}

	var cachedFilesSize: UInt64 {
		return UInt64(cachedFilesCount) * UInt64(pageSize)
	}

	var memoryUsedSize: UInt64 {
		return UInt64(memoryUsedCount) * UInt64(pageSize)
	}

	var wiredPercentage: Double {
		return Double(wiredPageCount) / Double(totalPageCount)
	}
	
	var activePercentage: Double {
		return Double(activePageCount) / Double(totalPageCount)
	}
	
	var inactivePercentage: Double {
		return Double(inactivePageCount) / Double(totalPageCount)
	}
	
	var freePercentage: Double {
		return Double(freePageCount) / Double(totalPageCount)
	}
	
	var compressedPercentage: Double {
		return Double(compressedPageCount) / Double(totalPageCount)
	}

	var appMemoryPercentage: Double {
		return Double(appMemoryCount) / Double(totalPageCount)
	}

	var cachedFilesPercentage: Double {
		return Double(cachedFilesCount) / Double(totalPageCount)
	}

	var memoryUsedPercentage: Double {
		return Double(memoryUsedCount) / Double(totalPageCount)
	}

}

extension MemorySample: CustomStringConvertible {
	var description: String {
		let wiredPercentage = wiredPercentage.formatted(.normalized)
		let activePercentage = activePercentage.formatted(.normalized)
		let inactivePercentage = inactivePercentage.formatted(.normalized)
		let freePercentage = freePercentage.formatted(.normalized)
		let compressedPercentage = compressedPercentage.formatted(.normalized)
		return "wired = \(wiredPercentage), active = \(activePercentage), inactive = \(inactivePercentage), free = \(freePercentage), compressed = \(compressedPercentage), pages in/out = \(pageInCount)/\(pageOutCount)"
	}
}

struct MemorySnapshot {
	let pageInCount: UInt64
	let pageOutCount: UInt64
}

// MARK: -

// cpu
//		per processor
//			user
//			system
//			idle
//			nice
// https://gist.github.com/paalgyula/47c8e37f6785bed6634d1cc1fb5697bc
// https://en.wikipedia.org/wiki/Apple_silicon

struct CoreSample {
	let system: Double
	let user: Double
	let idle: Double
	let nice: Double
	
	init() {
		system = 0.0
		user = 0.0
		idle = 0.0
		nice = 0.0
	}
	
	init(system: Double, user: Double, idle: Double, nice: Double) {
		self.system = system
		self.user = user
		self.idle = idle
		self.nice = nice
	}
	
	var usage: Double {
		return system + user + nice
	}
}

extension CoreSample: CustomStringConvertible {
	var description: String {
		return "system = \(system.formatted(.normalized)), user = \(user.formatted(.normalized)), idle = \(idle.formatted(.normalized)), nice = \(nice.formatted(.normalized))"
	}
}

struct CoreSnapshot {
	private(set) var systemTicks: Int32
	private(set) var userTicks: Int32
	private(set) var idleTicks: Int32
	private(set) var niceTicks: Int32
	
	init() {
		systemTicks = 0
		userTicks = 0
		idleTicks = 0
		niceTicks = 0
	}
	
	mutating func add(systemTicks: Int32, userTicks: Int32, idleTicks: Int32, niceTicks: Int32) {
		self.systemTicks += systemTicks
		self.userTicks += userTicks
		self.idleTicks += idleTicks
		self.niceTicks += niceTicks
	}
}

struct ProcessorSample {
	let coreSamples: [CoreSample]
}

struct ProcessorSnapshot {
	let coreSnapshots: [CoreSnapshot]
}

// MARK: -

struct GraphicsSample {
	let deviceUtilization: Double
	let memorySize: UInt64
}

extension GraphicsSample: CustomStringConvertible {
	var description: String {
		return "utilization = \(deviceUtilization.formatted(.normalized)), \(Double(memorySize).bytes()) used"
	}
}

// MARK: -

struct NetworkSample {
	let receivedPackets: UInt32
	let receivedErrors: UInt32
	let receivedBytes: UInt32

	let sentPackets: UInt32
	let sentErrors: UInt32
	let sentBytes: UInt32
	
	var receivedBitsPerSecond: UInt64 {
		return UInt64(receivedBytes) * 8
	}

	var sentBitsPerSecond: UInt64 {
		return UInt64(sentBytes) * 8
	}

}

extension FormatStyle where Self == FloatingPointFormatStyle<Double> {
	static var bitsPerSecond: FloatingPointFormatStyle<Double> {
		let style = FloatingPointFormatStyle<Double>()
		return style.scale(100).precision(.fractionLength(1))
	}
}

extension NetworkSample: CustomStringConvertible {
	var description: String {
		let received = Double(receivedBytes).bytes()
		let sent = Double(sentBytes).bytes()
		let receivedBps = Double(receivedBytes).bits()
		let sentBps = Double(sentBytes).bits()

		return "received = \(received) - \(receivedBps) [\(receivedPackets)/\(receivedErrors)], sent = \(sent) - \(sentBps) [\(sentPackets)/\(sentErrors)]"
	}
}

struct NetworkSnapshot {
	private(set) var receivedPackets: UInt32
	private(set) var receivedErrors: UInt32
	private(set) var receivedBytes: UInt64

	private(set) var sentPackets: UInt32
	private(set) var sentErrors: UInt32
	private(set) var sentBytes: UInt64
	
	init() {
		receivedPackets = 0
		receivedErrors = 0
		receivedBytes = 0

		sentPackets = 0
		sentErrors = 0
		sentBytes = 0
	}
	
	mutating func add(receivedPackets: UInt32, receivedErrors: UInt32, receivedBytes: UInt32, sentPackets: UInt32, sentErrors: UInt32, sentBytes: UInt32) {
		self.receivedPackets += receivedPackets
		self.receivedErrors += receivedErrors
		self.receivedBytes += UInt64(receivedBytes)

		self.sentPackets += sentPackets
		self.sentErrors += sentErrors
		self.sentBytes += UInt64(sentBytes)
	}
}

// MARK: -

struct LoadSample {
	
}

// MARK: -

struct VolumeSample {
	let totalBlocks: UInt64
	let availableBlocks: UInt64
	
	let blockSize: UInt32
	
	let name: String
	
	var totalSize: UInt64 {
		return totalBlocks * UInt64(blockSize)
	}
	
	var availableSize: UInt64 {
		return availableBlocks * UInt64(blockSize)
	}
	
	var usedSize: UInt64 {
		return (totalBlocks - availableBlocks) * UInt64(blockSize)
	}
	
	var usage: Double {
		return Double(totalBlocks - availableBlocks) / Double(totalBlocks)
	}
}

extension VolumeSample: CustomStringConvertible {
	var description: String {
		let total = totalSize.formatted(.byteCount(style: .file))
		let available = availableSize.formatted(.byteCount(style: .file))
		let used = usedSize.formatted(.byteCount(style: .file))

		return "'\(name)' \(available) available of \(total) [\(used) used]"
	}
}

struct StorageSample {
	let volumeSamples: [VolumeSample]
}

let collectorLogger = Logger(subsystem: "com.iconfactory.iPulse.Collector", category: "Collector")

class Collector {
	
	static let sampleCount = 10
	
	var memorySamples: [MemorySample] = []
	private var lastMemorySnapshot: MemorySnapshot?
	
	var processorSamples: [ProcessorSample] = []
	private var lastProcessorSnapshot: ProcessorSnapshot?
	
	var graphicsSamples: [GraphicsSample] = []
	
	var networkSamples: [NetworkSample] = []
	private var lastNetworkSnapshot: NetworkSnapshot?
	
	var storageSamples: [StorageSample] = []
	
	private let processorCoreMap: [Int]
	
	init() {
		var processorCoreMap: [Int] = []
		for (index, coreInfo) in processorCoreInfo.enumerated() {
			let items = [Int](repeating: index, count: coreInfo.count)
			processorCoreMap += items
		}
		self.processorCoreMap = processorCoreMap
	}
	
	struct CoreInfo {
		let count: Int
		let type: String
	}
	
	var processorCoreInfo: [CoreInfo] = {
		var result: [CoreInfo] = []
		
		let name = "hw.nperflevels"
		var size = MemoryLayout<Int>.size
		var numberOfLevels: Int = 0
		if sysctlbyname(name, &numberOfLevels, &size, nil, 0) != 0 {
			return result
		}
		
		for level in 0..<numberOfLevels {
			let countName = String(format: "hw.perflevel%d.logicalcpu", level)
			var countSize = MemoryLayout<Int>.size
			var count: Int = 0
			if sysctlbyname(countName, &count, &countSize, nil, 0) != 0 {
				count = 0
			}
			
			let typeName = String(format: "hw.perflevel%d.name", level)
			var typeSize = 256
			var typeCharacters = [CChar](repeating: 0, count: typeSize)
			let type: String
			if sysctlbyname(typeName, &typeCharacters, &typeSize, nil, 0) != 0 {
				type = ""
			}
			else {
				type = String(cString: typeCharacters)
			}
			
			let coreInfo = CoreInfo(count: count, type: type)
			result.insert(coreInfo, at: 0)
		}
		
		return result
	}()
	
	// use mach_absolute_time for sample time: https://developer.apple.com/library/archive/qa/qa1398/_index.html
	
	func collect() {
		collectMemory()
		collectNetwork()
		collectProcessor()
		collectDisks()
		//collectGraphics()
	}
	
	func reset() {
		lastMemorySnapshot = nil
		lastProcessorSnapshot = nil
		lastNetworkSnapshot = nil
	}
	
	private func collectMemory() {
		var machData = vm_statistics64_data_t()
		var count = mach_msg_type_number_t(MemoryLayout<vm_statistics64_data_t>.stride / MemoryLayout<integer_t>.stride)
		
		let result = withUnsafeMutablePointer(to: &machData) {
			$0.withMemoryRebound(to: integer_t.self, capacity: 1) {
				host_statistics64(mach_host_self(), HOST_VM_INFO64, $0, &count)
			}
		}
		guard result == KERN_SUCCESS else { return }
		
		let pageInDelta: UInt32
		let pageOutDelta: UInt32
		if let lastMemorySnapshot {
			pageInDelta = UInt32(machData.pageins - lastMemorySnapshot.pageInCount)
			pageOutDelta = UInt32(machData.pageouts - lastMemorySnapshot.pageOutCount)
		}
		else {
			pageInDelta = 0
			pageOutDelta = 0
		}
		
		lastMemorySnapshot = MemorySnapshot(pageInCount: machData.pageins, pageOutCount: machData.pageouts)
		
		let memorySample = MemorySample(wiredPageCount: machData.wire_count, activePageCount: machData.active_count, inactivePageCount: machData.inactive_count, freePageCount: machData.free_count, compressedPageCount: machData.compressor_page_count, internalPageCount: machData.internal_page_count, externalPageCount: machData.external_page_count, purgablePageCount: machData.purgeable_count, pageInCount: pageInDelta, pageOutCount: pageOutDelta)
		
		memorySamples.append(memorySample)
		while memorySamples.count > Self.sampleCount {
			memorySamples.remove(at: 0)
		}
		
		//collectorLogger.info("MEMORY: \(memorySample.description) - \(Double(memorySample.memoryUsedSize).bytes()) used - \(Double(memorySample.cachedFilesSize).bytes()) cached - \(Double(memorySample.memoryUsedSize + memorySample.cachedFilesSize + memorySample.freeSize).bytes()) sum - \(Double(memorySample.totalSize).bytes()) total")
		//collectorLogger.info("MEMORY: \(memorySample.description) - \(Double(memorySample.memoryUsedSize).bytes()) used - \(Double(memorySample.cachedFilesSize).bytes()) cached - \(Double(memorySample.totalSize).bytes()) total")
		
#if false
		do {
			let TASK_VM_INFO_COUNT = mach_msg_type_number_t(MemoryLayout<task_vm_info_data_t>.size / MemoryLayout<integer_t>.size)
			let TASK_VM_INFO_REV5_COUNT = mach_msg_type_number_t(MemoryLayout.offset(of: \task_vm_info_data_t.ledger_swapins)! / MemoryLayout<integer_t>.size)
			var info = task_vm_info_data_t()
			var count = TASK_VM_INFO_COUNT
			let kr = withUnsafeMutablePointer(to: &info) { infoPtr in
				infoPtr.withMemoryRebound(to: integer_t.self, capacity: Int(count)) { intPtr in
					task_info(mach_task_self_, task_flavor_t(TASK_VM_INFO), intPtr, &count)
				}
			}
			guard
				kr == KERN_SUCCESS,
				count >= TASK_VM_INFO_REV5_COUNT
			else { return }
			
			collectorLogger.info("MEMORY: device = \(info.device), internal = \(info.internal), external = \(info.external), reusable = \(info.reusable), compressed = \(info.compressed)")
		}
#endif
	}
	
	
	private func displayNetworkString(for flags: UInt32) -> String {
		let up = 			(flags & UInt32(IFF_UP) != 0 ? "up" : "  ")
		let broadcast = 	(flags & UInt32(IFF_BROADCAST) != 0 ? "br" : "  ")
		let debug =		 	(flags & UInt32(IFF_DEBUG) != 0 ? "db" : "  ")
		let loopback = 		(flags & UInt32(IFF_LOOPBACK) != 0 ? "lo" : "  ")
		let pointToPoint = 	(flags & UInt32(IFF_POINTOPOINT) != 0 ? "pp" : "  ")
		let running = 	(flags & UInt32(IFF_RUNNING) != 0 ? "rn" : "  ")
		
		return("\(up)|\(broadcast)|\(debug)|\(loopback)|\(pointToPoint)|\(running)")
	}
	
	// some interesting code to dig around in routes here: https://github.com/Shmoopi/iOS-System-Services/blob/master/System%20Services/Utilities/SSNetworkInfo.m
	private func collectNetwork() {
		var interfaceAddresses: UnsafeMutablePointer<ifaddrs>? = nil
		
		let error = getifaddrs(&interfaceAddresses)
		guard error == 0 else {
			collectorLogger.info("NETWORK: error = \(error)")
			return
		}
		
		var networkSnapshot = NetworkSnapshot()
		
		// TODO: Use Quinn's implementation https://developer.apple.com/forums/thread/734120
		var interfaceAddress = interfaceAddresses
		while interfaceAddress != nil {
			if let interfaceAddressData = interfaceAddress?.pointee {
				let socketData = interfaceAddressData.ifa_addr.pointee
				if socketData.sa_family == UInt8(AF_LINK) {
					let up = 		(interfaceAddressData.ifa_flags & UInt32(IFF_UP) != 0)
					let loopback = 	(interfaceAddressData.ifa_flags & UInt32(IFF_LOOPBACK) != 0)
					let running = 	(interfaceAddressData.ifa_flags & UInt32(IFF_RUNNING) != 0)
					
					if up && running && !loopback {
						let interfaceData = unsafeBitCast(interfaceAddressData.ifa_data, to: UnsafeMutablePointer<if_data>.self).pointee
						networkSnapshot.add(receivedPackets: interfaceData.ifi_ipackets, receivedErrors: interfaceData.ifi_ierrors, receivedBytes: interfaceData.ifi_ibytes, sentPackets: interfaceData.ifi_opackets, sentErrors: interfaceData.ifi_oerrors, sentBytes: interfaceData.ifi_obytes)
						
						//						let name: String = String(cString: interfaceAddressData.ifa_name)
						//						let displayFlags = displayNetworkString(for: interfaceAddressData.ifa_flags)
						//						let outputBytes = interfaceData.ifi_obytes
						//						let inputBytes = interfaceData.ifi_ibytes
						//						let outputErrors = interfaceData.ifi_oerrors
						//						let inputErrors = interfaceData.ifi_ierrors
						//						collectorLogger.info("NETWORK: \(displayFlags) \(name): inputBytes = \(inputBytes), outputBytes = \(outputBytes), errors = \(inputErrors)/\(outputErrors)")
					}
				}
				interfaceAddress = interfaceAddressData.ifa_next
			}
			else {
				interfaceAddress = nil
			}
		}
		
		freeifaddrs(interfaceAddresses)
		
		let deltaReceivedPackets: UInt32
		let deltaReceivedErrors: UInt32
		let deltaReceivedBytes: UInt32
		
		let deltaSentPackets: UInt32
		let deltaSentErrors: UInt32
		let deltaSentBytes: UInt32
		
		if let lastNetworkSnapshot {
			// NOTE: The deltas here can be negative since interfaces can disappear at lower the aggregate count. Since
			// a negative statistic is counterintuitive, the values are just reset to zero and will be correct after the
			// next collection.
			
			if networkSnapshot.receivedPackets > lastNetworkSnapshot.receivedPackets {
				deltaReceivedPackets = networkSnapshot.receivedPackets - lastNetworkSnapshot.receivedPackets
			}
			else {
				deltaReceivedPackets = 0
			}
			if networkSnapshot.receivedErrors > lastNetworkSnapshot.receivedErrors {
				deltaReceivedErrors = networkSnapshot.receivedErrors - lastNetworkSnapshot.receivedErrors
			}
			else {
				deltaReceivedErrors = 0
			}
			if networkSnapshot.receivedBytes > lastNetworkSnapshot.receivedBytes {
				deltaReceivedBytes = UInt32(networkSnapshot.receivedBytes - lastNetworkSnapshot.receivedBytes)
			}
			else {
				deltaReceivedBytes = 0
			}
			
			if networkSnapshot.sentPackets > lastNetworkSnapshot.sentPackets {
				deltaSentPackets = networkSnapshot.sentPackets - lastNetworkSnapshot.sentPackets
			}
			else {
				deltaSentPackets = 0
			}
			if networkSnapshot.sentErrors > lastNetworkSnapshot.sentErrors {
				deltaSentErrors = networkSnapshot.sentErrors - lastNetworkSnapshot.sentErrors
			}
			else {
				deltaSentErrors = 0
			}
			if networkSnapshot.sentBytes > lastNetworkSnapshot.sentBytes {
				deltaSentBytes = UInt32(networkSnapshot.sentBytes - lastNetworkSnapshot.sentBytes)
			}
			else {
				deltaSentBytes = 0
			}
		}
		else {
			deltaReceivedPackets = 0
			deltaReceivedErrors = 0
			deltaReceivedBytes = 0
			
			deltaSentPackets = 0
			deltaSentErrors = 0
			deltaSentBytes = 0
		}
		
		lastNetworkSnapshot = networkSnapshot
		
		let networkSample = NetworkSample(receivedPackets: deltaReceivedPackets, receivedErrors: deltaReceivedErrors, receivedBytes: deltaReceivedBytes, sentPackets: deltaSentPackets, sentErrors: deltaSentErrors, sentBytes: deltaSentBytes)
		//collectorLogger.info("NETWORK: \(networkSample.description)")
		networkSamples.append(networkSample)
		while networkSamples.count > Self.sampleCount {
			networkSamples.remove(at: 0)
		}
	}
	
	private func collectProcessor() {
		//collectorLogger.info("CPU: processorCoreInfo = \(processorCoreInfo)")
		
		var processorCount: natural_t = 0
		var processorInfoArray: processor_info_array_t? = nil
		var processorInfoCount: mach_msg_type_number_t = 0
		let result = host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &processorCount, &processorInfoArray, &processorInfoCount)
		guard result == KERN_SUCCESS else { return }
		guard let processorInfoArray else { return }
		
		let processorCoreCount = processorCoreInfo.count
		var coreSnapshots = [CoreSnapshot](repeating: CoreSnapshot(), count: processorCoreCount)
		
		var processorMap: [Int] = []
		for (index, coreInfo) in processorCoreInfo.enumerated() {
			let items = [Int](repeating: index, count: coreInfo.count)
			processorMap += items
		}
		
		for processor in 0..<processorCount {
			let index = Int32(processor) * CPU_STATE_MAX
			
			let systemTicks = processorInfoArray[Int(index + CPU_STATE_SYSTEM)]
			let userTicks = processorInfoArray[Int(index + CPU_STATE_USER)]
			let idleTicks = processorInfoArray[Int(index + CPU_STATE_IDLE)]
			let niceTicks = processorInfoArray[Int(index + CPU_STATE_NICE)]
			
			let coreIndex = processorCoreMap[Int(processor)]
			coreSnapshots[coreIndex].add(systemTicks: systemTicks, userTicks: userTicks, idleTicks: idleTicks, niceTicks: niceTicks)
			//collectorLogger.info("CPU: \(processor): systemTicks = \(systemTicks), userTicks = \(userTicks), idleTicks = \(idleTicks), niceTicks = \(niceTicks)")
		}
		
		var coreSamples: [CoreSample] = []
		
		if let lastProcessorSnapshot {
			for (index, coreSnapshot) in coreSnapshots.enumerated() {
				let deltaSystemTicks = coreSnapshot.systemTicks - lastProcessorSnapshot.coreSnapshots[index].systemTicks
				let deltaUserTicks = coreSnapshot.userTicks - lastProcessorSnapshot.coreSnapshots[index].userTicks
				let deltaIdleTicks = coreSnapshot.idleTicks - lastProcessorSnapshot.coreSnapshots[index].idleTicks
				let deltaNiceTicks = coreSnapshot.niceTicks - lastProcessorSnapshot.coreSnapshots[index].niceTicks
				
				let deltaTotalTicks = deltaSystemTicks + deltaUserTicks + deltaIdleTicks + deltaNiceTicks
				
				let system = Double(deltaSystemTicks) / Double(deltaTotalTicks)
				let user = Double(deltaUserTicks) / Double(deltaTotalTicks)
				let idle = Double(deltaIdleTicks) / Double(deltaTotalTicks)
				let nice = Double(deltaNiceTicks) / Double(deltaTotalTicks)
				
				let coreSample = CoreSample(system: system, user: user, idle: idle, nice: nice)
				coreSamples.append(coreSample)
			}
		}
		else {
			coreSamples = [CoreSample](repeating: CoreSample(), count: coreSnapshots.count)
		}
		
		lastProcessorSnapshot = ProcessorSnapshot(coreSnapshots: coreSnapshots)
		
		let processorSample = ProcessorSample(coreSamples: coreSamples)
		processorSamples.append(processorSample)
		while processorSamples.count > Self.sampleCount {
			processorSamples.remove(at: 0)
		}
		
//		for (index, coreSample) in coreSamples.enumerated() {
//			collectorLogger.info("CPU: \(self.processorCoreInfo[index].type) \(coreSample.description)")
//		}
		
		// use bitPattern? https://stackoverflow.com/a/48630296/132867
		vm_deallocate(mach_task_self_, vm_address_t(processorInfoArray.pointee), vm_size_t(processorInfoCount))
	}
	
	private func collectLoad() {
		//let cpuEntries = IORegistryEntryFromPath()
		
		let port = mach_host_self()
		
		var machData = host_load_info_data_t()
		var count = mach_msg_type_number_t(MemoryLayout<host_load_info_data_t>.stride / MemoryLayout<integer_t>.stride)
		
		let result = withUnsafeMutablePointer(to: &machData) {
			$0.withMemoryRebound(to: integer_t.self, capacity: 1) {
				host_statistics64(port, HOST_LOAD_INFO, $0, &count)
			}
		}
		guard result == KERN_SUCCESS else { return }
		
		let load = Double(machData.avenrun.0) / Double(LOAD_SCALE)
		collectorLogger.info("LOAD: average.0 = \(load)")
		
	}
	
	
	private func displayDiskString(for flags: UInt32) -> String {
		let readOnly = 		(flags & UInt32(MNT_RDONLY) != 0 ? "ro" : "  ")
		//let synchronous =	(flags & UInt32(MNT_SYNCHRONOUS) != 0 ? "sy" : "  ")
		//let noExec =		(flags & UInt32(MNT_NOEXEC) != 0 ? "nx" : "  ")
		//let noSuid = 		(flags & UInt32(MNT_NOSUID) != 0 ? "ns" : "  ")
		let noDev = 		(flags & UInt32(MNT_NODEV) != 0 ? "nd" : "  ")
		//let union = 		(flags & UInt32(MNT_UNION) != 0 ? "un" : "  ")
		//let async = 		(flags & UInt32(MNT_ASYNC) != 0 ? "as" : "  ")
		//let protect	= 		(flags & UInt32(MNT_CPROTECT) != 0 ? "cp" : "  ")
		//let exported = 		(flags & UInt32(MNT_EXPORTED) != 0 ? "ex" : "  ")
		let removable = 	(flags & UInt32(MNT_REMOVABLE) != 0 ? "rm" : "  ")
		//let quarantine = 	(flags & UInt32(MNT_QUARANTINE) != 0 ? "qu" : "  ")
		
		//let local = 		(flags & UInt32(MNT_LOCAL) != 0 ? "lo" : "  ")
		//let quota = 		(flags & UInt32(MNT_QUOTA) != 0 ? "qu" : "  ")
		let root = 			(flags & UInt32(MNT_ROOTFS) != 0 ? "rt" : "  ")
		let doVolfs = 		(flags & UInt32(MNT_DOVOLFS) != 0 ? "dv" : "  ")
		
		let dontBrowse = 	(flags & UInt32(MNT_DONTBROWSE) != 0 ? "db" : "  ")
		//let ignoreOwner = 	(flags & UInt32(MNT_IGNORE_OWNERSHIP) != 0 ? "io" : "  ")
		//let autoMounted = 	(flags & UInt32(MNT_AUTOMOUNTED) != 0 ? "am" : "  ")
		let journaled = 	(flags & UInt32(MNT_JOURNALED) != 0 ? "jo" : "  ")
		//let noUserExAttr = 	(flags & UInt32(MNT_NOUSERXATTR) != 0 ? "nu" : "  ")
		//let deferWrite = 	(flags & UInt32(MNT_DEFWRITE) != 0 ? "dw" : "  ")
		//let multiLabel = 	(flags & UInt32(MNT_MULTILABEL) != 0 ? "ml" : "  ")
		//let noFollow = 		(flags & UInt32(MNT_NOFOLLOW) != 0 ? "nf" : "  ")
		let noAccessTime = 	(flags & UInt32(MNT_NOATIME) != 0 ? "na" : "  ")
		let snapshot = 		(flags & UInt32(MNT_SNAPSHOT) != 0 ? "ss" : "  ")
		//let strictTime = 	(flags & UInt32(MNT_STRICTATIME) != 0 ? "st" : "  ")
		//let unknownPerm = 	(flags & UInt32(MNT_UNKNOWNPERMISSIONS) != 0 ? "uk" : "  ")
		
		//let update = 		(flags & UInt32(MNT_UPDATE) != 0 ? "up" : "  ")
		//let noBlock = 		(flags & UInt32(MNT_NOBLOCK) != 0 ? "nb" : "  ")
		//let reload = 		(flags & UInt32(MNT_RELOAD) != 0 ? "rl" : "  ")
		//let force = 		(flags & UInt32(MNT_FORCE) != 0 ? "fo" : "  ")
		
		//		return("\(synchronous) \(noExec) \(noSuid) \(noDev) \(union) \(async) \(protect) \(exported) \(removable) \(quarantine)")
		//		return("\(readOnly)\(local)\(dontBrowse)\(ignoreOwner)\(autoMounted)\(journaled)\(noUserExAttr)\(deferWrite)\(multiLabel)")
		//		return("\(readOnly)\(noFollow)\(noAccessTime)\(snapshot)\(strictTime)\(unknownPerm)")
		//		return("\(readOnly)\(update)\(noBlock)\(reload)\(force)")
		return("\(readOnly)|\(root)|\(dontBrowse)|\(noDev)|\(removable)|\(doVolfs)|\(journaled)|\(noAccessTime)|\(snapshot)")
	}
	
	private func collectDisks() {
		var volumeSamples: [VolumeSample] = []
		
		var mountInfo: UnsafeMutablePointer<statfs>? = nil
		let mountSize = getmntinfo(&mountInfo, MNT_NOWAIT)
		if let mountInfo {
			for i in 0..<mountSize {
				let index = Int(i)
				let mountData = mountInfo[index]
				let flags = mountData.f_flags
				let readOnly = (flags & UInt32(MNT_RDONLY) != 0)
				let rootFilesystem = (flags & UInt32(MNT_ROOTFS) != 0)
				let removableDevice = (flags & UInt32(MNT_REMOVABLE) != 0)
				
				if rootFilesystem || (removableDevice && !readOnly) {
					let blockSize = mountData.f_bsize
					let totalBlocks = mountData.f_blocks
					let availableBlocks = mountData.f_bavail
					
					//					let toName = String.fromTuple(tuple: mountData.f_mntonname) ?? "N/A"
					//					let type = String.fromTuple(tuple: mountData.f_fstypename) ?? "N/A"
					//					let typeNumber = mountData.f_type
					//					let owner = mountData.f_owner
					//					let totalSize = totalBlocks *  UInt64(blockSize)
					//					let freeSize = availableBlocks *  UInt64(blockSize)
					//					let displayFlags = displayDiskString(for: flags)
					
					let volumeName: String
					if rootFilesystem {
						volumeName = "Device"
					}
					else {
						if removableDevice && !readOnly {
							let toName = String.fromTuple(tuple: mountData.f_mntonname) ?? "N/A"
							if let url = URL(string: toName) {
								volumeName = url.lastPathComponent
							}
							else {
								volumeName = "Removable Storage"
							}
						}
						else {
							volumeName = "Other"
						}
					}
					
					let volumeSample = VolumeSample(totalBlocks: totalBlocks, availableBlocks: availableBlocks, blockSize: blockSize, name: volumeName)
					//collectorLogger.info("DISK: \(volumeSample.description)")
					
					volumeSamples.append(volumeSample)
					//					let free = ByteCountFormatter.string(fromByteCount: Int64(freeSize), countStyle: .file)
					//					let total = ByteCountFormatter.string(fromByteCount: Int64(totalSize), countStyle: .file)
					//					collectorLogger.info("DISK: \(displayFlags), type = \(type) [\(typeNumber)]\t'\(volumeName)' \(free) of \(total)")
				}
			}
		}
		
		let storageSample = StorageSample(volumeSamples: volumeSamples)
		storageSamples.append(storageSample)
		while storageSamples.count > Self.sampleCount {
			storageSamples.remove(at: 0)
		}
	}
	
	private func collectGraphics() {
		let graphicsUtilization = queryGraphicsUtilization()
		let deviceUtilization = graphicsUtilization / 100.0
		let graphicsSample = GraphicsSample(deviceUtilization: deviceUtilization, memorySize: 0)
		//collectorLogger.info("GRAPHICS: \(graphicsSample.description)")
		
		graphicsSamples.append(graphicsSample)
		while graphicsSamples.count > Self.sampleCount {
			graphicsSamples.remove(at: 0)
		}
	}
	
}

// from: https://gist.github.com/oxycoder/3c224a4d390f37264d5156b80312f49a
extension String {
	// convert to String from Tuple
	static func fromTuple<T>(tuple:T) -> String? {
		let reflection = Mirror(reflecting: tuple)
		var characters: [CChar] = []
		for child in reflection.children {
			if let character = child.value as? CChar {
				characters.append(character)
			}
		}
		return String(cString: characters)
	}
}

