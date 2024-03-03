//
//  Transfer.swift
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 3/2/24.
//

import Foundation

struct Transfer: Codable {
	
	var cpuUsage: Float? // 0.0 to 1.0 (inclusive)
	
	var networkReceivedBps: UInt64?
	var networkSentBps: UInt64?

	var diskReadBytes: UInt64?
	var diskWriteBytes: UInt64?

}
