//
//  CacheManager.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/12/8.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import UIKit
import Cache

class CacheManager {
    static let shared: Storage? = {
        do {
            let s = try Storage(diskConfig: DiskConfig(name: cacheName), memoryConfig: nil)
            return s
        }catch {
            console.debug(error)
            return nil
        }
    }()
}
