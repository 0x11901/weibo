//
//  UIImageView+WJKAddition.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import SDWebImage

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
    public func wjk_setImageWith(urlStr: String, placeHolderName: String?) {
        if let url = URL(string: urlStr) {
            if let placeHolder = placeHolderName {
                sd_setImage(with: url, placeholderImage: UIImage(named: placeHolder))
            } else {
                sd_setImage(with: url)
            }
        }
    }
}
