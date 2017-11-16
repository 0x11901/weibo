//
//  AppDelegate+AppService.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/16.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import UIKit

extension AppDelegate {
    
    /// 初始化 window
    internal func initWindow() {
        window = UIWindow()
        let root = WBRootViewController()
        window?.rootViewController = root
        window?.makeKeyAndVisible()
        
        UITabBar.appearance().tintColor = globalColor
        UINavigationBar.appearance().tintColor = globalColor
    }
    
}
