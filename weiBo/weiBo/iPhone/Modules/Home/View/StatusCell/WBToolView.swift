//
//  WBToolView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBToolView: UIView {
    
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupVIew()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

extension WBToolView {
    
    fileprivate func setupVIew() {
        backgroundColor = UIColor(patternImage: UIImage(named: "timeline_card_bottom_line_highlighted")!)
        let retweetedButton =  UIButton(title: "", fontSize: 10, color: UIColor.gray, image: "timeline_icon_retweet")
        let commontButton = UIButton(title: "30", fontSize: 10, color: UIColor.gray, image: "timeline_icon_comment")
        let prizeButton = UIButton(title: "30", fontSize: 10, color: UIColor.gray, image: "timeline_icon_unlike")
        
        addSubview(retweetedButton)
        addSubview(commontButton)
        addSubview(prizeButton)
        
        retweetedButton.snp.makeConstraints { (make) in
            make.left.equalTo(self)
            make.top.equalTo(self)
            make.bottom.equalTo(self)
        }
        commontButton.snp.makeConstraints { (make) in
            make.left.equalTo(retweetedButton.snp.right)
            make.top.equalTo(self)
            make.bottom.equalTo(self)
            make.width.equalTo(retweetedButton)
        }
        prizeButton.snp.makeConstraints { (make) in
            make.right.equalTo(self)
            make.left.equalTo(commontButton.snp.right)
            make.top.equalTo(self)
            make.bottom.equalTo(self)
            make.width.equalTo(commontButton)
        }
    }
    
}
