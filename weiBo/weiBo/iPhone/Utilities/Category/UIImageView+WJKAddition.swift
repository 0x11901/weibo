//
//  UIImageView+WJKAddition.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import Kingfisher

extension UIImageView {
    
    /// 传imageName初始化一个imageView
    ///
    /// - Parameter imageName: 图片的名字
    convenience init(imageName: String) {
        self.init()
        self.image = UIImage(named: imageName)
    }
}

extension UIImageView {
    public func setImage(urlStr: String, placeHolderName: Any? = nil,progressBlock: DownloadProgressBlock? = nil,completionHandler: CompletionHandler? = nil) {
        guard let url = URL(string: urlStr) else {
            return
        }
        if let name = placeHolderName as? String,let placeHolder = UIImage(named: name) {
            self.kf.setImage(with: url, placeholder: placeHolder, progressBlock: progressBlock, completionHandler: completionHandler)
        }else if let placeHolder = placeHolderName as? Placeholder {
            self.kf.setImage(with: url, placeholder: placeHolder, progressBlock: progressBlock,completionHandler: completionHandler)
        }else{
            self.kf.setImage(with: url, progressBlock: progressBlock, completionHandler: completionHandler)
        }
        
    }
}
