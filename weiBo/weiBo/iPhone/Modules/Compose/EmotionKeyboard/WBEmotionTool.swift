//
//  WBEmotionTool.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/6.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBEmotionTool: NSObject {
    static let shared = WBEmotionTool()

    private var bundlePath: String {
        return Bundle.main.path(forResource: "Emoticons", ofType: "bundle")!
    }

    /// 默认表情的地址
    private var defaultPath: String {
        return bundlePath + "/Contents/Resources/default/info.plist"
    }

    /// emoji表情的地址
    private var emojiPath: String {
        return bundlePath + "/Contents/Resources/emoji/info.plist"
    }

    /// emoji表情的地址
    private var lxhPath: String {
        return bundlePath + "/Contents/Resources/lxh/info.plist"
    }

    /// 根据path解析出表情的model
    func parseEmotionModel(path: String) -> [WBEmotionModel] {
        var array = [WBEmotionModel]()
        if let dictArray = NSArray(contentsOfFile: path) {
            for dict in dictArray {
                guard let dictionary = dict as? [String: Any] else {
                    continue
                }
                let model = WBEmotionModel(dict: dictionary)
                if model.type == 0 {
                    let folderPath = (path as NSString).deletingLastPathComponent
                    model.fullPath = "\(folderPath)/\(model.png!)"
                }
                array.append(model)
            }
        }
        return array
    }

    /// 将表情model根据cell来分组
    func devideEmotions(emotions: [WBEmotionModel]) -> [[WBEmotionModel]] {
        var resultArray = [[WBEmotionModel]]()
        let pageNum = (emotions.count - 1) / 20 + 1
        for i in 0 ..< pageNum {
            if i + 1 == pageNum {
                let range = NSRange(location: i * 20, length: emotions.count - i * 20)
                resultArray.append((emotions as NSArray).subarray(with: range) as! [WBEmotionModel])
            } else {
                let range = NSRange(location: i * 20, length: 20)
                resultArray.append((emotions as NSArray).subarray(with: range) as! [WBEmotionModel])
            }
        }
        return resultArray
    }

    func emotionsDataSource() -> [[[WBEmotionModel]]] {
        return [
            devideEmotions(emotions: parseEmotionModel(path: defaultPath)),
            devideEmotions(emotions: parseEmotionModel(path: defaultPath)),
            devideEmotions(emotions: parseEmotionModel(path: emojiPath)),
            devideEmotions(emotions: parseEmotionModel(path: lxhPath)),
        ]
    }
}
