//
//  main.swift
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 2/24/24.
//

import Foundation

// TODO: Use signal handler to exit RunLoop: https://swiftpackageregistry.com/Genaro-Chris/SignalHandler

// TODO: Parse arguments: https://www.swift.org/blog/argument-parser/

autoreleasepool {
	let controller = Controller()
	controller.start()
	RunLoop.current.run()
}

