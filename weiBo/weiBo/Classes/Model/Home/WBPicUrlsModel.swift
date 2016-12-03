//
//  WBPicUrlsModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/30.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBPicUrlsModel: NSObject {
    var thumbnail_pic: String?{
        didSet{
            middle_pic = thumbnail_pic?.replacingOccurrences(of: "/thumbnail/", with: "/bmiddle/")
        }
    }
    var middle_pic: String?
    override var description: String{
        return yy_modelDescription()
    }
}
