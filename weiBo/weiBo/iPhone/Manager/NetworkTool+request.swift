//
//  NetworkTool+request.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/27.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import Alamofire
import UIKit

extension NetworkManager {
    @discardableResult
    public func requestForAccessToken(code: String, networkCompletionHandler: @escaping ([String: Any]?) -> Void) -> Cancellable? {
        let parameters = [
            "client_id": appKey,
            "client_secret": appSecrect,
            "grant_type": "authorization_code",
            "code": code,
            "redirect_uri": redirectURI,
        ]
        return post(url: "https://api.weibo.com/oauth2/access_token", parameters: parameters, networkCompletionHandler: {
            networkCompletionHandler($0.value as? [String: Any])
        })
    }

    @discardableResult
    public func requestForUserInfo(parameters: [String: Any], networkCompletionHandler: @escaping ([String: Any]?) -> Void) -> Cancellable? {
        // 接口升级后，对未授权本应用的uid，将无法获取其个人简介、认证原因、粉丝数、关注数、微博数及最近一条微博内容。
        return get(url: "https://api.weibo.com/2/users/show.json", parameters: parameters, networkCompletionHandler: {
            networkCompletionHandler($0.value as? [String: Any])
        })
    }

    @discardableResult
    public func requestForHomeStatus(sinceId: Int = 0, maxId: Int = 0, networkCompletionHandler: @escaping ([String: Any]?) -> Void) -> Cancellable? {
        if let access_token = WBUserAccountModel.shared.access_token {
            let parameters = [
                "access_token": access_token,
                "since_id": NSNumber(value: sinceId),
                "max_id": NSNumber(value: maxId),
            ] as [String: Any]
            return get(url: "https://api.weibo.com/2/statuses/home_timeline.json", parameters: parameters, networkCompletionHandler: {
                networkCompletionHandler($0.value as? [String: Any])
            })
        }
        return nil
    }

    @discardableResult
    public func requestForWeather(parameters: [String: Any], networkCompletionHandler: @escaping ([String: Any]?) -> Void) -> Cancellable? {
        return get(url: "https://api.seniverse.com/v3/weather/now.json?key=hpejjcjphjea8ogq&language=zh-Hans&unit=c", parameters: parameters, networkCompletionHandler: {
            networkCompletionHandler($0.value as? [String: Any])
        })
    }

    @discardableResult
    public func postStatus(status: String, image: UIImage? = nil, success: @escaping ((_ response: Any?) -> Void), failure _: @escaping ((_ error: Error) -> Void)) -> Cancellable? {
        if let access_token = WBUserAccountModel.shared.access_token {
            let parameters = [
                "access_token": access_token,
                "status": status,
            ]

            if image != nil {
                post(url: "https://upload.api.weibo.com/2/statuses/upload.json", parameters: parameters, image: image!, networkCompletionHandler: { response in
                    success(response)
                })
            } else {
                return post(url: "https://api.weibo.com/2/statuses/update.json", parameters: parameters, networkCompletionHandler: { response in
                    success(response)
                })
            }
        }
        return nil
    }
}
