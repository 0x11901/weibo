//
//  WBStatusViewModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBStatusViewModel: NSObject {
    var status: WBStatusModel
    var verifiedType: UIImage?
    var verifiedLevel: UIImage?
    var sourceString: String?
    var timeString: String?
    var retweetedString: String?
    var rowHeight: CGFloat = 0
    var firstImageSize: CGSize = CGSize.zero
    var pic_urls: [WBPicUrlsModel]?

    init(status: WBStatusModel) {
        self.status = status
        super.init()
        getType()
        getLevel()
        dealWithSource()
        dealWithTime()
        dealWithRetweetedText()
        calculateRowHeight()
    }
}

extension WBStatusViewModel {
    fileprivate func calculateRowHeight() {
        if let pics = status.pic_urls {
            pic_urls = pics
        }
        if let pics = status.retweeted_status?.pic_urls {
            pic_urls = pics
        }

        if var height = pic_urls?.count, height != 0 {
            height = (height - 1) / 3 + 1
            let heightf = CGFloat(height)
            rowHeight = widthHeight * heightf + margin * (heightf - 1.0)
        }
    }

    fileprivate func dealWithRetweetedText() {
        if let text = status.retweeted_status?.text, text.count > 0, let userName = status.retweeted_status?.user?.screen_name {
            retweetedString = "@\(userName):\(text)"
        }
    }

    fileprivate func dealWithTime() {
        guard let sinaTime = status.created_at else {
            return
        }
        let date = Date.dateFromSinaFormat(sinaDateString: sinaTime)
        timeString = date.requiredTimeString()
    }

    fileprivate func getType() {
        if let type = status.user?.verified_type {
            switch type {
            case 0:
                verifiedType = #imageLiteral(resourceName: "avatar_vip")
            case 2, 3, 5:
                verifiedType = #imageLiteral(resourceName: "avatar_enterprise_vip")
            case 220:
                verifiedType = #imageLiteral(resourceName: "avatar_grassroot")
            default:
                verifiedType = nil
            }
        }
    }

    fileprivate func getLevel() {
        if let level = status.user?.verified_level, level <= 7 && level > 0 {
            verifiedLevel = UIImage(named: "common_icon_membership_level\(level)")
        }
    }

    fileprivate func dealWithSource() {
        if let count = (status.source?.count), count > 0, let start = status.source?.range(of: "\">")?.upperBound, let end = status.source?.range(of: "</a>")?.lowerBound, let str = status.source {
            sourceString = String(describing: str[start ..< end])
        }
    }
}
