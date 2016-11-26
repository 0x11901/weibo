//
//  UIImageView+WJKAddition.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

extension UIImageView {
    
    /// 传imageName初始化一个imageView
    ///
    /// - Parameter imageName: 图片的名字
    convenience init(imageName: String) {
        self.init()
        self.image = UIImage(named: imageName)
    }
}
