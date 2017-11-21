//
//  WBUserAccount.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/27.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBUserAccountModel: NSObject {
    static let shared = WBUserAccountModel()
    
    @objc var access_token: String?
    @objc var uid:String?
    @objc var screen_name: String?
    @objc var avatar_large:String?
    @objc var expires_in: Double = 0 {
        didSet{
            expires_date = Date(timeInterval: expires_in, since: Date())
        }
    }
    @objc var expires_date: Date?
    var isLogin: Bool {
        return access_token != nil && expires_date?.compare(Date()) == .orderedDescending
    }
    
    override init() {
        super.init()
        self.loadAccount()
    }

}

extension WBUserAccountModel {
    public func saveAccount (dictionary: [String : Any]) {
        self.setValuesForKeys(dictionary)
        let dict = dictionaryWithValues(forKeys: ["access_token","uid","screen_name","avatar_large","expires_date"])
        UserDefaults.standard.set(dict, forKey: accountKey)
    }
    
    /// 从内存中读取模型
    fileprivate func loadAccount() {
        if let dictionary: [String : Any] = UserDefaults.standard.value(forKey: accountKey) as? [String : Any] {
            self.setValuesForKeys(dictionary)
        }
    }
    
    override func setValue(_ value: Any?, forUndefinedKey key: String) {
        console.debug(key)
        console.debug(value)
    }

}

/*
 ["lang": zh-cn, "verified_reason_url": , "province": 51, "bi_followers_count": 3, "friends_count": 144, "verified_source_url": , "allow_all_comment": 1, "id": 3224302683, "pagefriends_count": 0, "urank": 15, "profile_image_url": http://tva2.sinaimg.cn/crop.0.19.456.456.50/c02ef45bgw1ejr4rs0xtij20fj0dc0ty.jpg, "following": 0, "url": , "city": 7, "star": 0, "geo_enabled": 0, "screen_name": 我就看看-不说话_, "description": 一直觉得这张图片的眼神犀利！, "followers_count": 36, "mbtype": 0, "created_at": Mon Jan 14 11:04:12 +0800 2013, "remark": , "block_app": 0, "remind_in": 157679999, "domain": wjk930726, "class": 1, "favourites_count": 0, "ptype": 0, "credit_score": 80, "idstr": 3224302683, "user_ability": 4096, "profile_url": wjk930726, "weihao": , "follow_me": 0, "access_token": 2.00z6qMWD0i16lVc3ec3fa768ZqhsRB, "statuses_count": 439, "allow_all_act_msg": 0, "name": 我就看看-不说话_, "verified_type": -1, "block_word": 0, "verified_reason": , "avatar_hd": http://tva2.sinaimg.cn/crop.0.19.456.456.1024/c02ef45bgw1ejr4rs0xtij20fj0dc0ty.jpg, "gender": m, "uid": 3224302683, "expires_in": 157679999, "online_status": 0, "status": {
 "attitudes_count" = 0;
 "biz_feature" = 0;
 "bmiddle_pic" = "http://ww1.sinaimg.cn/bmiddle/c02ef45bjw1f4rt3lrf7vj20ku1en44k.jpg";
 "comments_count" = 0;
 "created_at" = "Sun Jun 12 01:38:59 +0800 2016";
 "darwin_tags" =     (
 );
 favorited = 0;
 geo = "<null>";
 "gif_ids" = "";
 hasActionTypeCard = 0;
 "hot_weibo_tags" =     (
 );
 id = 3985357516677509;
 idstr = 3985357516677509;
 "in_reply_to_screen_name" = "";
 "in_reply_to_status_id" = "";
 "in_reply_to_user_id" = "";
 isLongText = 0;
 "is_show_bulletin" = 0;
 mid = 3985357516677509;
 mlevel = 0;
 "original_pic" = "http://ww1.sinaimg.cn/large/c02ef45bjw1f4rt3lrf7vj20ku1en44k.jpg";
 "pic_urls" =     (
 {
 "thumbnail_pic" = "http://ww1.sinaimg.cn/thumbnail/c02ef45bjw1f4rt3lrf7vj20ku1en44k.jpg";
 }
 );
 "positive_recom_flag" = 0;
 "reposts_count" = 0;
 source = "<a href=\"http://app.weibo.com/t/feed/3KeSKP\" rel=\"nofollow\">WeicoPro</a>";
 "source_allowclick" = 0;
 "source_type" = 1;
 text = "\U5c31\U559c\U6b22\U7b80\U5355\U9ad8\U6548\Uff01\U6211\U5728\U7528@Weico\U5fae\U535a\U5ba2\U6237\U7aef\Uff0c\U667a\U80fd\U591c\U95f4\U6a21\U5f0f\U3001\U4e2a\U6027\U5b9a\U5236\U9605\U8bfb\U73af\U5883\U3001\U8fd8\U6709\U79bb\U7ebf\U6a21\U5f0f\U8282\U7701\U6d41\U91cf\Uff01\U60f3\U4f53\U9a8c\U66f4\U597d\Uff1f\U6233\U94fe\U63a5\U4e0b\U8f7dWeico Pro 4:http://t.cn/RymHptd";
 textLength = 147;
 "text_tag_tips" =     (
 );
 "thumbnail_pic" = "http://ww1.sinaimg.cn/thumbnail/c02ef45bjw1f4rt3lrf7vj20ku1en44k.jpg";
 truncated = 0;
 userType = 0;
 visible =     {
 "list_id" = 0;
 type = 0;
 };
 }, "verified_trade": , "location": 四川 绵阳, "verified_source": , "verified": 0, "mbrank": 0, "avatar_large": http://tva2.sinaimg.cn/crop.0.19.456.456.180/c02ef45bgw1ejr4rs0xtij20fj0dc0ty.jpg] */
