//
//  WBStatusListViewModel.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/2.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import Kingfisher

class WBStatusListViewModel: NSObject {
    lazy var dataSource: [WBStatusViewModel] = []
    
    func loadDate(isPull: Bool,callBack: @escaping (Bool)->() ) {
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
            
            guard let json = dictionary["statuses"] else{
                print("获取json错误")
                return
            }
            
            guard let status = NSArray.yy_modelArray(with: WBStatusModel.self, json: json) else{
                print("json转模型错误")
                return
            }
            
            guard let statues = status as? [WBStatusModel] else{
                print("statues类型错误")
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
                        print("第一张图url有误")
                        return
                    }
                    ImageDownloader.default.downloadImage(with: ulrT,completionHandler: { (downloadImage, err, _, _) in
                        guard err == nil ,var size = downloadImage?.size else{
                            print("第一张图下载有误")
                            
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
            
            
            if isPull {
                self.dataSource = arrM + self.dataSource
            }else{
                self.dataSource.removeLast()
                self.dataSource += arrM
            }
            
            group.notify(queue: DispatchQueue.main) {
                callBack(true)
            }
        }
   
    }
}
