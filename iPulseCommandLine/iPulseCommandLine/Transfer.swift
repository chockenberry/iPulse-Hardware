//
//  Transfer.swift
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 3/2/24.
//

import Foundation

struct Transfer: Codable {
	
	var cpuUsage: Float? // 0.0 to 1.0 (inclusive)
	
	var networkReceivedPackets: UInt32?
	var networkReceivedBps: UInt64?
	var networkSentPackets: UInt32?
	var networkSentBps: UInt64?

	var diskReadCount: UInt64?
	var diskReadBytes: UInt64?
	var diskWriteCount: UInt64?
	var diskWriteBytes: UInt64?

	var memoryPhysicalSize: UInt64?
	var memoryWiredSize: UInt64?
	var memoryAppSize: UInt64?
	var memoryCompressedSize: UInt64?
	
	var loadOneMinute: Float?
	var loadFiveMinutes: Float?
	var loadFifteenMinutes: Float?
	
	var uptime: Int?
}
