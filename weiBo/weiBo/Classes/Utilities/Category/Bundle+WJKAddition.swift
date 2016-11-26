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
