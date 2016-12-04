//
//  WBOriginalView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import SDWebImage

class WBOriginalView: UIView {
    var status: WBStatusViewModel?{
        didSet{
            guard let model = status,let iconSring = model.status.user?.avatar_large else {
                print("模型传值错误")
                return
            }
            
            if let pictures = status?.status.pic_urls, pictures.count > 0, let height = status?.rowHeight{
                pictureView.snp.updateConstraints { (make) in
                    make.top.equalTo(textLabel.snp.bottom).offset(12)
                    make.size.equalTo(CGSize(width: screenWidth - 24, height: height))
                }
                pictureView.viewModel = status
            }else{
                pictureView.snp.updateConstraints { (make) in
                    make.top.equalTo(textLabel.snp.bottom).offset(0)
                    make.size.equalTo(CGSize.zero)
                }
            }
            

            let url = URL(string: iconSring)
            SDWebImageManager.shared().downloadImage(with: url, options: [], progress: nil) { (downloadImage, err, _, _, _) in
                downloadImage?.createCornerImage(size: CGSize(width: 35, height: 35), callBack: { (image) in
                    self.iconImageView.image = image
                })
            }
            userNameLabel.text = model.status.user?.screen_name
            levelIcon.image = model.verifiedLevel
            vipIcon.image = model.verifiedType
            timeLabel.text = model.timeString
            sourceLabel.text = model.sourceString
            textLabel.text = model.status.text
        }
    }
    lazy var iconImageView: UIImageView = UIImageView()
    lazy var userNameLabel: UILabel = UILabel(title: "", fontSize: 15)
    lazy var levelIcon: UIImageView = UIImageView()
    lazy var vipIcon: UIImageView  = UIImageView()
    lazy var timeLabel: UILabel = UILabel(title: "", fontSize: 10, fontColor: UIColor.lightGray)
    lazy var sourceLabel: UILabel = UILabel(title: "", fontSize: 10, fontColor: UIColor.lightGray)
    lazy var textLabel: UILabel = UILabel(title: "", fontSize: 15)
    lazy var pictureView: WBPictureView = WBPictureView()
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupVIew()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

extension WBOriginalView {
    
    fileprivate func setupVIew() {
        backgroundColor = UIColor.white
        addSubview(iconImageView)
        addSubview(userNameLabel)
        addSubview(levelIcon)
        addSubview(vipIcon)
        addSubview(timeLabel)
        addSubview(sourceLabel)
        addSubview(textLabel)
        addSubview(pictureView)
        
        UIImage(named: "avatar_default_big")?.createCornerImage(size: CGSize(width: 35, height: 35), callBack: { (image) in
            self.iconImageView.image = image
        })
        
        iconImageView.snp.makeConstraints { (make) in
            make.leading.top.equalTo(12)
            make.size.equalTo(CGSize(width: 35, height: 35))
        }
        userNameLabel.snp.makeConstraints { (make) in
            make.top.equalTo(iconImageView)
            make.leading.equalTo(iconImageView.snp.trailing).offset(10)
        }
        levelIcon.snp.makeConstraints { (make) in
            make.centerY.equalTo(userNameLabel)
            make.leading.equalTo(userNameLabel.snp.trailing).offset(5)
            make.size.equalTo(CGSize(width: 14, height: 14))
        }
        vipIcon.snp.makeConstraints { (make) in
            make.bottom.equalTo(iconImageView).offset(5)
            make.trailing.equalTo(iconImageView).offset(5)
        }
        timeLabel.snp.makeConstraints { (make) in
            make.bottom.equalTo(iconImageView)
            make.leading.equalTo(userNameLabel)
        }
        sourceLabel.snp.makeConstraints { (make) in
            make.leading.equalTo(timeLabel.snp.trailing).offset(10)
            make.bottom.equalTo(timeLabel)
        }
        textLabel.snp.makeConstraints { (make) in
            make.top.equalTo(iconImageView.snp.bottom).offset(12)
            make.leading.equalTo(self).offset(12)
            make.trailing.equalTo(self).offset(-12)
        }
        pictureView.snp.makeConstraints { (make) in
            make.top.equalTo(textLabel.snp.bottom).offset(12)
            make.leading.equalTo(self).offset(12)
            make.bottom.equalTo(self).offset(-12)
            make.size.equalTo(CGSize(width: screenWidth - 24, height: screenWidth - 24))
        }
    }
    
}