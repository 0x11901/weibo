//
//  WBPictureView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBPictureView: UIView {
    var viewModel: WBStatusViewModel? {
        didSet{
            guard let vm = viewModel,let pictures = vm.pic_urls else {
                print("vm not exist")
                return
            }
            for obj in subviews {
                obj.isHidden = true
            }
            var isOne = false
            var isFour = false
            if pictures.count == 1{
                isOne = true
            }else if pictures.count == 4 {
                isFour = true
            }
            for item in pictures.enumerated() {
                var tag = item.offset
                if isOne {
                    subviews.first?.frame.size = vm.firstImageSize
                }else{
                    subviews.first?.frame.size = CGSize(width: widthHeight, height: widthHeight)
                }
                if isFour,tag > 1 {
                    tag += 1
                }
                guard let urlStr = item.element.thumbnail_pic else {
                    return
                }
                for obj in subviews {
                    if obj.tag == tag {
                        let imageV = obj as! UIImageView
                        imageV.isHidden = false
                        imageV.setImage(urlStr: urlStr, placeHolderName: "avatar_default_big")
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
        backgroundColor = super.backgroundColor
        clipsToBounds = true
        let frame = CGRect(x: 0, y: 0, width: widthHeight, height: widthHeight)
        for i in 0..<9 {
            let row = CGFloat(i / 3)
            let col = CGFloat(i % 3)
            let imageView = UIImageView(frame: frame.offsetBy(dx: col * (widthHeight + margin), dy: row * (widthHeight + margin)))
            imageView.contentMode = .scaleAspectFill
            imageView.clipsToBounds = true
            imageView.tag = i
            imageView.isUserInteractionEnabled = true
            imageView.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(imageAction(tap:))))
            addSubview(imageView)
        }
    }
    
}

extension WBPictureView {
    
    @objc fileprivate func imageAction(tap:UITapGestureRecognizer) {
        guard var index = tap.view?.tag,let picArr = viewModel?.pic_urls else{
            print("image tag beyond bounds")
            return
        }
        if viewModel?.pic_urls?.count == 4,index > 2 {
            index -= 1
        }
        
        let ocArr = picArr as NSArray
        
        if let urls = ocArr.value(forKeyPath: "middle_pic") as? [String]{
            let userinfo:[AnyHashable : Any] = [indexKey : index,urlsKey : urls]
            NotificationCenter.default.post(name: clickThumbImage, object: self, userInfo: userinfo)
        }
        
    }
    
}
