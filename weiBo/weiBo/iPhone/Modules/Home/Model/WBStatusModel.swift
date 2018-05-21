//
//  WBStatusModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import HandyJSON
import UIKit

class WBStatusModel: HandyJSON {
    var created_at: String?
    var id: Int = 0
    var text: String?
    var user: WBUser?
    var source: String?
    var retweeted_status: WBStatusModel?
    var pic_urls: [WBPicUrlsModel]?

    required init() {}
}
