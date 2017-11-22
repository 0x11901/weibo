//
//  WBPicUrlsModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/30.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import HandyJSON

struct WBPicUrlsModel: HandyJSON {
    var thumbnail_pic: String?
    var middle_pic: String?
    
    mutating func didFinishMapping() {
        middle_pic = thumbnail_pic?.replacingOccurrences(of: "/thumbnail/", with: "/bmiddle/")
    }
}
