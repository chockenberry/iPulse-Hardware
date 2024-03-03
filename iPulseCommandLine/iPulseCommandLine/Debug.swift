//
//  Debug.swift
//
//  Created by Craig Hockenberry on 3/15/17.
//  Updated by Craig Hockenberry on 2/20/24.

//  Usage:
//
//    SplineReticulationManager.swift:
//
//    func reticulationFunction() -> Float {
//        debugLog() // prints "SplineReticulationManager: reticulationFunction() called"
//
//        let splineCount = Int.random(in: 0...2000)
//        debugLog("reticulating \(splineCount) splines") // prints "SplineReticulationManager: reticulationFunction() reticulating 1337 splines"
//
//        return debugResult(Float.random(in: 0...1)) // prints "SplineReticulationManager: reticulationFunction() returned: 5.8008"
//    }


import Foundation
import OSLog

let defaultLogger = Logger()

public func releaseLog(_ message: String = "called", file: String = #file, function: String = #function) {
	defaultLogger.info("\(URL(fileURLWithPath: file, isDirectory: false).deletingPathExtension().lastPathComponent, privacy: .public): \(function, privacy: .public) \(message, privacy: .public)")
}

public func debugLog(_ message: String = "called", file: String = #file, function: String = #function) {
#if DEBUG
	defaultLogger.debug("\(URL(fileURLWithPath: file, isDirectory: false).deletingPathExtension().lastPathComponent, privacy: .public): \(function, privacy: .public) \(message, privacy: .public)")
#endif
}

@discardableResult
public func debugResult<T>(_ result: T, file: String = #file, function: String = #function) -> T {
	debugLog("returned: \(result)", file: file, function: function)
	return result
}
