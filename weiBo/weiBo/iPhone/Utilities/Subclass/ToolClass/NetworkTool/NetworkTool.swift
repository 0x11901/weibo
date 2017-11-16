//
//  NetworkTool.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import AFNetworking
import Alamofire

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
    
    func POST(URLString: String, parameters: Any?,image: UIImage,
              success: @escaping (_ responseObject: Any?)->(),
              failure: @escaping (_ error: Error) -> ()) {
        self.post(URLString, parameters: parameters, constructingBodyWith: { (formData) in
            guard let data = UIImagePNGRepresentation(image) else{
                print("picture tansform to binary error")
                return
            }
            formData.appendPart(withFileData: data, name: "pic", fileName: "helloWorld.wjk", mimeType: "application/octet-stream")
        }, progress: nil, success: { (_, response) in
            success(response)
        }, failure: { (_, error) in
            failure(error)
        })
    }
    
}















