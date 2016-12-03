//
//  UIImage+WJKAddition.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/30.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

extension UIImage {
    
    public func createCornerImage(size: CGSize = CGSize.zero,backgroundColor: UIColor = UIColor.clear,callBack: @escaping (_ cornerImage: UIImage) -> ()) {
        OperationQueue().addOperation {
            let rect = CGRect(origin: CGPoint.zero, size: size)
            UIGraphicsBeginImageContext(size)
            backgroundColor.setFill()
            UIRectFill(rect)
            UIBezierPath(ovalIn: rect).addClip()
            self.draw(in: rect)
            let image = UIGraphicsGetImageFromCurrentImageContext()
            UIGraphicsEndImageContext()
            
            OperationQueue.main.addOperation {
                if let image = image {
                    callBack(image)
                }
            }
        }
    }
    
}
