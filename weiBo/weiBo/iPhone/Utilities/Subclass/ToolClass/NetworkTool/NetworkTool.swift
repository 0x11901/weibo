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
    
    static let alamofire = { () -> NetworkTool in
        let instance = NetworkTool(baseURL: nil)
        return instance
    }()
    
    //    '`URL` cannot be formed with the string (for example, if the string contains characters that are illegal in a URL, or is an empty string).'
    
    struct NetworkToolError: Error {
        enum ErrorKind {
            case invalidURLString
        }
        
        let kind: ErrorKind
        
    }
    
    func getss(URLString: String,
             parameters: [String: Any]?,
             success: @escaping (_ responseObject: Any?)->(),
             failure: @escaping (_ error: Error) -> ()) {
        guard let url = URL(string: URLString) else {
            let error: Error = NetworkToolError(kind: .invalidURLString)
            failure(error)
            return
        }
        

        
        Alamofire.request(url, method: .get, parameters: parameters, encoding: JSONEncoding.default).responseJSON { response in
            switch response.result {
            case .success(_):
                success(response)
            case .failure(let e):
                failure(e)
            }
        }
    }
    
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















