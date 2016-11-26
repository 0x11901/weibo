//
//  AppDelegate.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?


    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey : Any]? = nil) -> Bool {
        // Override point for customization after application launch.
        window = UIWindow()
        let root = WBRootViewController()
        window?.rootViewController = root
        window?.makeKeyAndVisible()

        UITabBar.appearance().tintColor = globalColor
        return true
    }
    
}

