//
//  String+Emoji.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/6.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import Foundation

extension String {

    /// 将十六进制的编码转为emoji字符
    ///
    /// - Parameter intCode: 十六进制的编码
    /// - Returns: emoji字符
    public static func emoji(intCode: UInt) -> String {
        
        var symbol: UInt = ((((0x808080F0 | (intCode & 0x3F000) >> 4) | (intCode & 0xFC0) << 10) | (intCode & 0x1C0000) << 18) | (intCode & 0x3F) << 24)
        
        if let string: NSString = NSString(bytes: &symbol, length: MemoryLayout.size(ofValue: symbol), encoding: 4) {
            return string as String
        }
        
        let code = unichar(intCode)
        let hhh = "\(code)"
        return hhh
    }

    /// 将十六进制的编码转为emoji字符
    ///
    /// - Parameter stringCode: 十六进制的字符串
    /// - Returns: emoji字符
    public static func emoji(stringCode: String) -> String {
        
        let scanner: Scanner = Scanner(string: stringCode)
    
        var intCode: UInt32 = 0
        
        scanner.scanHexInt32(&intCode)
    
        return self.emoji(intCode: UInt(intCode))
        
    }
    
    /// 将十六进制的字符串转为emoji字符
    ///
    /// - Returns: emoji字符
    public func emoji() -> String {
        return String.emoji(stringCode: self)
    }

    /// 判断是否为emoji字符
    ///
    /// - Returns: 是否为emoji字符
    public func isEmoji() -> Bool {
        let emojiString = self as NSString
        var returnValue = false;
        let hs: unichar = emojiString.character(at: 0)
        // surrogate pair
        if (0xd800 <= hs && hs <= 0xdbff) {
            if (emojiString.length > 1) {
                let ls: unichar = emojiString.character(at: 1)
                let uc: Int = ((Int(hs) - 0xd800) * 0x400) + (Int(ls) - 0xdc00) + 0x10000
                if 0x1d000 <= uc && uc <= 0x1f77f {
                    returnValue = true;
                }
            }
        } else if (emojiString.length > 1) {
            let ls: unichar = emojiString.character(at: 1)
            if (ls == 0x20e3) {
                returnValue = true;
            }
        } else {
            // non surrogate
            if (0x2100 <= hs && hs <= 0x27ff) {
                returnValue = true;
            } else if (0x2B05 <= hs && hs <= 0x2b07) {
                returnValue = true;
            } else if (0x2934 <= hs && hs <= 0x2935) {
                returnValue = true;
            } else if (0x3297 <= hs && hs <= 0x3299) {
                returnValue = true;
            } else if (hs == 0xa9 || hs == 0xae || hs == 0x303d || hs == 0x3030 || hs == 0x2b55 || hs == 0x2b1c || hs == 0x2b1b || hs == 0x2b50) {
                returnValue = true;
            }
        }
        
        return returnValue
    }
}
