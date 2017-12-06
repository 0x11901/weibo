//
//  AppDelegate.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import SnapKit
import XCGLogger
import Toast_Swift

let console: XCGLogger = XCGLogger.default

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {
    
    var window: UIWindow?
    
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey : Any]? = nil) -> Bool {
        
        //初始化logger
        initLogger(logger: console)
        
        //初始化 window
        initWindow()
        //一些请求
        requestForSomething()
        
        return true
    }
    
}
