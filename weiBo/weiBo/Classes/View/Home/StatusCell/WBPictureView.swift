//
//  WBPictureView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBPictureView: UIView {
    var pictures: [WBPicUrlsModel]? {
        didSet{
            if let pictures = pictures {
                for obj in subviews {
                    obj.isHidden = true
                }
                
                for item in pictures.enumerated() {
                    let tag = item.offset
                    guard let urlStr = item.element.thumbnail_pic else {
                        return
                    }
                    for obj in subviews {
                        if obj.tag == tag {
                            print(urlStr)
                            let imageV = obj as! UIImageView
                            imageV.wjk_setImageWith(urlStr: urlStr, placeHolderName: "avatar_default_big")
                        }
                    }
                }
            }
        }
    }
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupView()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

extension WBPictureView {
    
    fileprivate func setupView() {
        backgroundColor = UIColor.randomColor()
        clipsToBounds = true
        let frame = CGRect(x: 0, y: 0, width: widthHeight, height: widthHeight)
        for i in 0..<9 {
            let row = CGFloat(i / 3)
            let col = CGFloat(i % 3)
            let imageView = UIImageView(frame: frame.offsetBy(dx: col * (widthHeight + margin), dy: row * (widthHeight + margin)))
            imageView.backgroundColor = UIColor.randomColor()
            imageView.contentMode = .scaleAspectFill
            imageView.tag = i
            addSubview(imageView)
        }
    }
    
}

