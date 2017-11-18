//
//  AppDelegate+AppService.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/16.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import UIKit
import XCGLogger

extension AppDelegate {
    
    /// 初始化 window
    internal func initWindow() {
        window = UIWindow()
        let root = WBRootViewController()
        window?.rootViewController = root
        window?.makeKeyAndVisible()
        
        UITabBar.appearance().tintColor = globalColor
        UINavigationBar.appearance().tintColor = globalColor
        
        if #available(iOS 11.0, *) {
            UIScrollView.appearance().contentInsetAdjustmentBehavior = .never
        }
    }
    
    /// 初始化XCGLogger对象，并进行设置
    internal func initLogger(logger: XCGLogger) {
        logger.setup(level: .debug, showThreadName: true, showLevel: true, showFileNames: true, showLineNumbers: true, writeToFile: "path/to/file", fileLevel: .debug)
    }
    
}
