//
//  UILabel+WJKAddition.swift
//  contacts
//
//  Created by 王靖凯 on 2016/11/21.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

extension UILabel {
    @objc convenience init(title: String?, fontSize: CGFloat = 14, fontColor: UIColor = UIColor.black, textAlignment: NSTextAlignment = .natural, numberOfLines: NSInteger = 0) {
        self.init()
        text = title
        font = UIFont.systemFont(ofSize: fontSize)
        textColor = fontColor
        self.textAlignment = textAlignment
        self.numberOfLines = numberOfLines
    }
}
