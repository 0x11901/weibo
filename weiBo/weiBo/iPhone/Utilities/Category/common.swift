//
//  common.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

/// 全局颜色
let globalColor = UIColor.colorWithHex(hex: 0xF98200)

/// 屏幕的宽度
let screenWidth = UIScreen.main.bounds.size.width

///屏幕的高度
let screenHeight = UIScreen.main.bounds.size.height

///分辨率的倍数
let screenScale = UIScreen.main.scale

///OAuth
let appKey = "469365216"
let appSecrect = "e48bbe54548acda95d19fffdd73aa011"
let redirectURI = "http://steamcommunity.com/id/wjk930726/"

///Account
let loginSuccess = "loginSuccess"
let accountKey = "accountKey"

///new feature
let isNewFeatureKey = "isNewFeature"

///边距
let margin: CGFloat = 12.0
let widthHeight = (screenWidth - 4 * margin) / CGFloat(3)

///通知
let clickThumbImage: Notification.Name = Notification.Name(rawValue: "clickSamllImage")
let indexKey = "indexKey"
let urlsKey = "urlsKey"
let addOrDeleteNotification = Notification.Name(rawValue: "addOrDeleteNotification")
let clickHyperlink: Notification.Name = Notification.Name(rawValue: "clickHyperlink")
let hyperlinkTextKey = "hyperlinkTextKey"
