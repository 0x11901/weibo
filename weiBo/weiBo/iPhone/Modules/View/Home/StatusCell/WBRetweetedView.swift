//
//  WBRetweetedView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBRetweetedView: UIView {
    var retweetedStatus: WBStatusViewModel? {
        didSet{
            if let pictures = retweetedStatus?.status.retweeted_status?.pic_urls, pictures.count > 0, let height = retweetedStatus?.rowHeight{
                pictureView.snp.updateConstraints { (make) in
                    make.bottom.equalTo(self).offset(-12)
                    make.height.equalTo(height).priority(999)
                    make.width.equalTo(screenWidth - 24)
                }
                pictureView.viewModel = retweetedStatus
            }else{
                
                pictureView.snp.updateConstraints { (make) in
                    make.bottom.equalTo(self).offset(0)
                    make.height.equalTo(0).priority(999)
                    make.width.equalTo(0)
                }
            }
            
            textLabel.text = retweetedStatus?.retweetedString
            guard let count = retweetedStatus?.retweetedString?.characters.count, count > 0 else {
                textLabel.snp.updateConstraints { (make) in
                    make.top.equalTo(self).offset(0)
                    make.bottom.equalTo(pictureView.snp.top).offset(0)
                }
                return
            }
            textLabel.snp.updateConstraints { (make) in
                make.top.equalTo(self).offset(12)
                make.bottom.equalTo(pictureView.snp.top).offset(-12)
            }
        }
    }
    lazy var textLabel: AXLabel = AXLabel(title: "", fontSize: 13, fontColor: UIColor.darkGray)
    lazy var pictureView: WBPictureView = WBPictureView()
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupVIew()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

extension WBRetweetedView {
    fileprivate func setupVIew() {
        backgroundColor = UIColor.colorWithHex(hex: 0xF7F6F7)
        addSubview(textLabel)
        addSubview(pictureView)
        
        textLabel.snp.makeConstraints { (make) in
            make.top.equalTo(self).offset(12)
            make.leading.equalTo(self).offset(12)
            make.trailing.equalTo(self).offset(-12)
            make.bottom.equalTo(pictureView.snp.top).offset(-12)
        }
        pictureView.snp.makeConstraints { (make) in
            make.leading.equalTo(self).offset(12)
            make.bottom.equalTo(self).offset(-12)
            make.height.equalTo(screenWidth - 24).priority(999)
            make.width.equalTo(screenWidth - 24)
        }
    }
    
}
