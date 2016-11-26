//
//  NetworkTool.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import AFNetworking

class NetworkTool: AFHTTPSessionManager {
    static let shared = { () -> NetworkTool in
        let instance = NetworkTool(baseURL: nil)
        instance.responseSerializer.acceptableContentTypes?.insert("text/plain")
        return instance
    }()
    
    func GET(URLString: String, parameters: Any?,
                         success: @escaping (_ responseObject: Any?)->(),
                         failure: @escaping (_ error: Error) -> ()) {
        let success = {
            (task: URLSessionDataTask, response: Any?) in
            success(response)
        }
        
        let failure = {
            (task: URLSessionDataTask?, error: Error) in
            failure(error)
        }
        
        self.get(URLString, parameters: parameters, progress: nil, success: success, failure: failure)
    }
    
    func POST(URLString: String, parameters: Any?,
                          success: @escaping (_ responseObject: Any?)->(),
                          failure: @escaping (_ error: Error) -> ()) {
        let success = {
            (task: URLSessionDataTask, response: Any?) in
            success(response)
        }
        
        let failure = {
            (task: URLSessionDataTask?, error: Error) in
            failure(error)
        }
        self.post(URLString, parameters: parameters, progress: nil, success: success, failure: failure)
    }
}
