//
//  Bundle+WJKAddition.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import Foundation

extension Bundle {
    var nameSpace: String {
        return Bundle.main.infoDictionary!["CFBundleName"] as! String
    }
}

extension Bundle {
    class func isNewFeature() -> (Bool) {
        let currentVersion = Bundle.main.infoDictionary!["CFBundleShortVersionString"] as! String
        guard let oldVersion = UserDefaults.standard.value(forKey: isNewFeatureKey) as? String else {
            UserDefaults.standard.set(currentVersion, forKey: isNewFeatureKey)
            return true
        }
        if oldVersion == currentVersion {
            return false
        }else{
            UserDefaults.standard.set(currentVersion, forKey: isNewFeatureKey)
            return true
        }
    }
}
