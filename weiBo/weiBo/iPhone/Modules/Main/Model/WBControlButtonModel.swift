//
//  WBControlButtonModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/30.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import HandyJSON
import UIKit

struct WBControlButtonModel: HandyJSON {
    var title: String?
    var icon: String?
}

extension WBControlButtonModel {
    public static func getControlButtonArray() -> [WBControlButtonModel]? {
        if let url = Bundle.main.url(forResource: "compose", withExtension: "json"),
            let data = try? Data(contentsOf: url),
            let jsonString = String(data: data, encoding: .utf8),
            let target = [WBControlButtonModel].deserialize(from: jsonString) as? [WBControlButtonModel] {
            return target
        }
        return nil
    }
}
