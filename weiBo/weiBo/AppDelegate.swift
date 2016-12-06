//
//  AppDelegate.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import SnapKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?


    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey : Any]? = nil) -> Bool {
        window = UIWindow()
        
//        let root = WBRootViewController()
//        window?.rootViewController = root
        
        let composeController = WBComposeViewController()
        let composeNav = UINavigationController(rootViewController: composeController)
        window?.rootViewController = composeNav
        
        window?.makeKeyAndVisible()

        UITabBar.appearance().tintColor = globalColor
        UINavigationBar.appearance().tintColor = globalColor
        
        print(NSHomeDirectory())
        return true
    }
    
}

