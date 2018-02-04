//
//  WBStatusListViewModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/2.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

/*
 CacheManager.shared?.async.object(ofType: Data.self, forKey: key, completion: { (result) in
 switch result {
 case .value(let data):
 let json = try? JSONSerialization.jsonObject(with: data, options: [])
 console.debug(json ?? "nothing")
 case .error(let error):
 console.error(error)
 }
 })
 */

import UIKit
import Kingfisher

class WBStatusListViewModel: NSObject {
    lazy var dataSource: [WBStatusViewModel] = []
    
    func loadDate(isPull: Bool,callBack: @escaping (Bool)->() ) {
        // TODO: 在此处进行json缓存
        var sinceId = 0
        var maxId = 0
        if dataSource.count == 0 {
            sinceId = 0
        }else if isPull {
            sinceId = (dataSource.first?.status.id)!
        }else{
            maxId = (dataSource.last?.status.id)!
        }
        NetworkManager.shared.requestForHomeStatus(sinceId: sinceId, maxId: maxId) { (obj) in
            guard let dictionary = obj else {
                console.debug("request for home status error!")
                callBack(false)
                return
            }
            
            // 应在此处保存json
            // 反解析然后保存？
            // sinceid放在数组中
            var idArray = [String]()
            
            if let since = dictionary["since_id"] as? Int {
                let key = "JSON:\(since)"
                //把key保存在数组中，读取的时候遍历数组即可判断
                if let data = try? JSONSerialization.data(withJSONObject: dictionary, options: []) {
                    CacheManager.shared?.async.setObject(data, forKey: key, completion: { (_) in
                        idArray.append(key)
                    })
                }
            }
            
            guard let json = dictionary["statuses"] as? [Any] else{
                console.debug("获取json错误")
                callBack(false)
                return
            }
            
            guard let status = [WBStatusModel].deserialize(from: json) else {
                console.debug("json转模型错误")
                callBack(false)
                return
            }
            
            guard let statues = status as? [WBStatusModel] else{
                console.debug("statues类型错误")
                callBack(false)
                return
            }
            
            let group = DispatchGroup()
            var arrM: [WBStatusViewModel] = []
            for obj in statues {
                let viewModel = WBStatusViewModel(status: obj)
                arrM.append(viewModel)
                
                if let pic_urls = viewModel.pic_urls, pic_urls.count == 1 {
                    group.enter()
                    guard let url = pic_urls.first?.thumbnail_pic,let ulrT = URL(string: url) else {
                        console.debug("第一张图url有误")
                        return
                    }
                    ImageDownloader.default.downloadImage(with: ulrT,completionHandler: { (downloadImage, err, _, _) in
                        guard err == nil ,var size = downloadImage?.size else{
                            console.debug("第一张图下载有误")
                            return
                        }
                        size.height = size.height * 2
                        size.width = size.width * 2
                        viewModel.firstImageSize = size
                        viewModel.rowHeight = size.height
                        group.leave()
                    })
                }
            }
            
            var isCancel = false
            
            group.notify(queue: DispatchQueue.main) {
                if !isCancel {
                    if isPull {
                        self.dataSource = arrM + self.dataSource
                    }else{
                        self.dataSource.removeLast()
                        self.dataSource += arrM
                    }
                    callBack(true)
                }
            }
            
            // TODO: 如果网络请求失败，没做错误处理
            DispatchQueue.global().async {
                let result = group.wait(timeout: .now() + 30)
                switch result {
                case .timedOut:
                    isCancel = true
                    callBack(false)
                default:
                    do {}
                }
            }
            
        }
        
    }
}
