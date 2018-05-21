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
    @objc var uid: String?
    @objc var screen_name: String?
    @objc var avatar_large: String?
    @objc var expires_in: Double = 0 {
        didSet {
            expires_date = Date(timeInterval: expires_in, since: Date())
        }
    }

    @objc var expires_date: Date?
    var isLogin: Bool {
        return access_token != nil && expires_date?.compare(Date()) == .orderedDescending
    }

    override init() {
        super.init()
        loadAccount()
    }
}

extension WBUserAccountModel {
    public func saveAccount(dictionary: [String: Any]) {
        setValuesForKeys(dictionary)
        let dict = dictionaryWithValues(forKeys: ["access_token", "uid", "screen_name", "avatar_large", "expires_date"])
        UserDefaults.standard.set(dict, forKey: accountKey)
    }

    /// 从内存中读取模型
    fileprivate func loadAccount() {
        if let dictionary: [String: Any] = UserDefaults.standard.value(forKey: accountKey) as? [String: Any] {
            setValuesForKeys(dictionary)
        }
    }

    override func setValue(_: Any?, forUndefinedKey _: String) {
    }
}
