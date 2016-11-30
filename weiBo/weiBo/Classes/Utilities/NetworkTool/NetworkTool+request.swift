//
//  NetworkTool+request.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/27.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import Foundation

extension NetworkTool {
    public func requestForAccessToken(code: String,success: @escaping ((_ response: Any?) -> ()),failure: @escaping ((_ error: Error) -> ())) {
        let parameters = ["client_id": appKey,
                          "client_secret": appSecrect,
                          "grant_type": "authorization_code",
                          "code": code,
                          "redirect_uri": redirectURI]
        
        self.POST(URLString: "https://api.weibo.com/oauth2/access_token", parameters: parameters, success: { (response) in
            success(response)
        }, failure: { (error) in
            failure(error)
        })
    }
    
    public func requestForUserInfo(parameters: [String : Any],success: @escaping ((_ response: Any?) -> ()),failure: @escaping ((_ error: Error) -> ())) {
 
        self.GET(URLString: "https://api.weibo.com/2/users/show.json", parameters: parameters, success:{ (response) in
            success(response)
        }, failure: { (error) in
            failure(error)
        })
    }
    
    public func requestForHomeStatus(sinceId: Int = 0,maxId: Int = 0,success: @escaping ((_ response: Any?) -> ()),failure: @escaping ((_ error: Error) -> ())) {
        if let access_token = WBUserAccountModel.shared.access_token {
            let parameters = ["access_token": access_token,
                              "since_id": NSNumber(value: sinceId),
                              "max_id": NSNumber(value: maxId),
                              "count": "100"] as [String : Any]
            self.GET(URLString: "https://api.weibo.com/2/statuses/home_timeline.json", parameters: parameters, success:{ (response) in
                success(response)
            }, failure: { (error) in
                failure(error)
            })
        }
    }
}
