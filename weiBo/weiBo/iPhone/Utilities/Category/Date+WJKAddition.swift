//
//  Date+WJKAddition.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/30.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

private let dateFromat = DateFormatter()
private let calendar = Calendar.current

extension Date {
    
    static func dateFromSinaFormat(sinaDateString: String) -> Date {
        dateFromat.locale = Locale(identifier: "en")
        dateFromat.dateFormat = "EEE MMM dd HH:mm:ss zzz yyyy"
        return dateFromat.date(from: sinaDateString)!
    }
    
    public func requiredTimeString() -> String{
        if calendar.isDateInToday(self) {
            let secends = -Int(self.timeIntervalSinceNow)
            if secends < 60 {
                return "刚刚"
            }else if secends < 3600 {
                return "\(secends / 60)分钟前"
            }else{
                return "\(secends / 3600)小时前"
            }
        }else if calendar.isDateInYesterday(self){
            dateFromat.locale = Locale(identifier: "en")
            dateFromat.dateFormat = "昨天 HH：mm"
            return "\(dateFromat.string(from: self))"
        }else{
            let thisYear = calendar.component(.year, from: Date())
            let hisYear = calendar.component(.year, from: self)
            if thisYear == hisYear {
                dateFromat.dateFormat = "MM-dd HH: mm"
                dateFromat.locale = Locale(identifier: "en")
                return "\(dateFromat.string(from: self))"
            }else{
                dateFromat.dateFormat = "yyyy-MM-dd HH: mm"
                dateFromat.locale = Locale(identifier: "en")
                return "\(dateFromat.string(from: self))"
            }
        }
    }
    
}














