//
//  WBUserInfoModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/26.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import HandyJSON
import UIKit

class WBUserInfo {
    var weatherInfo: WeatherInfo?

    static let shared = WBUserInfo()
    private init() {}
}

struct Results: HandyJSON {
    var results: [WeatherInfo]?
}

struct WeatherInfo: HandyJSON {
    var last_update: String?
    var location: Location?
    var now: Now?
}

struct Location: HandyJSON {
    var name: String?
    var country: String?
    var path: String?
}

struct Now: HandyJSON {
    var code: Int?
    var temperature: Int8?
    var text: String?
}
