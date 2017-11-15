//
//  UIColor+WJKAddition.swift
//  coverFlow
//
//  Created by 王靖凯 on 2016/11/22.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

extension UIColor {
    class func randomColor()->UIColor {
        let red = CGFloat(arc4random_uniform(256))
        let yellow = CGFloat(arc4random_uniform(256))
        let blue = CGFloat(arc4random_uniform(256))
        return UIColor(red: red / 255.0, green: yellow / 255.0, blue: blue / 255.0, alpha: 1.0)
    }
    
    class func colorWithHex(hex: Int32)->UIColor {
        let red = CGFloat((hex & 0xff0000) >> 16)
        let yellow = CGFloat((hex & 0x00ff00) >> 8)
        let blue = CGFloat(hex & 0x0000ff)
        return UIColor(red: red / 255.0, green: yellow / 255.0, blue: blue / 255.0, alpha: 1.0)
    }
}
