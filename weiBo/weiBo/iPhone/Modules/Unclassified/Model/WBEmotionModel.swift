//
//  WBEmotionModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/6.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBEmotionModel: NSObject {
    /// 简体中文
    var chs: String?
    /// 繁体中文
    var cht: String?
    /// 表情的图片
    var png: String?
    /// 表情的gif图片
    var gif: String?
    /// emoji的16进制的字符串
    var code: String?
    /// 表情的类型0: 图片表情, 1. emoji表情
    var type: Int = 0
    /// 全路径
    var fullPath: String?

    init(dict: [String: Any]) {
        super.init()
        setValuesForKeys(dict)
    }

    override var description: String {
        return dictionaryWithValues(forKeys: ["chs", "cht", "png", "gif", "code", "type", "fullPath"]).description
    }
}
