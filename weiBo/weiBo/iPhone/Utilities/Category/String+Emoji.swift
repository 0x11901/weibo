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
        var symbol: UInt = ((((0x8080_80F0 | (intCode & 0x3F000) >> 4) | (intCode & 0xFC0) << 10) | (intCode & 0x1C0000) << 18) | (intCode & 0x3F) << 24)

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

        return emoji(intCode: UInt(intCode))
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
        var returnValue = false
        let hs: unichar = emojiString.character(at: 0)
        // surrogate pair
        if 0xD800 <= hs && hs <= 0xDBFF {
            if emojiString.length > 1 {
                let ls: unichar = emojiString.character(at: 1)
                let uc: Int = ((Int(hs) - 0xD800) * 0x400) + (Int(ls) - 0xDC00) + 0x10000
                if 0x1D000 <= uc && uc <= 0x1F77F {
                    returnValue = true
                }
            }
        } else if emojiString.length > 1 {
            let ls: unichar = emojiString.character(at: 1)
            if ls == 0x20E3 {
                returnValue = true
            }
        } else {
            // non surrogate
            if 0x2100 <= hs && hs <= 0x27FF {
                returnValue = true
            } else if 0x2B05 <= hs && hs <= 0x2B07 {
                returnValue = true
            } else if 0x2934 <= hs && hs <= 0x2935 {
                returnValue = true
            } else if 0x3297 <= hs && hs <= 0x3299 {
                returnValue = true
            } else if hs == 0xA9 || hs == 0xAE || hs == 0x303D || hs == 0x3030 || hs == 0x2B55 || hs == 0x2B1C || hs == 0x2B1B || hs == 0x2B50 {
                returnValue = true
            }
        }

        return returnValue
    }
}
