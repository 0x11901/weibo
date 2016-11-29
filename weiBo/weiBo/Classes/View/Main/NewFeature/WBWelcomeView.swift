//
//  WBWelcomeView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/27.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBWelcomeView: UIView {
    fileprivate lazy var iconView: UIImageView = {
        let icon = UIImageView()
        icon.wjk_setImageWith(urlStr: WBUserAccountModel.shared.avatar_large!, placeHolderName: "avatar_default_big")
        return icon
    }()
    
    fileprivate lazy var welcomeLabel: UILabel = UILabel(title: "\(WBUserAccountModel.shared.screen_name!), 欢迎归来!耶!", fontSize: 17, fontColor: UIColor.lightGray, textAlignment: .center)
    
    override init(frame: CGRect) {
        super.init(frame: UIScreen.main.bounds)
        setupUI()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func didMoveToWindow() {
        addAnimation()
    }
}

extension WBWelcomeView {
    fileprivate func setupUI() {
        backgroundColor = UIColor.white
        addSubview(iconView)
        addSubview(welcomeLabel)
        
        iconView.layer.masksToBounds = true
        iconView.layer.cornerRadius = 45
        
        iconView.snp.makeConstraints { (make) in
            make.centerX.equalTo(self)
            make.centerY.equalTo(self).offset(-100)
            make.size.equalTo(CGSize(width: 90, height: 90))
        }
        welcomeLabel.snp.makeConstraints { (make) in
            make.centerX.equalTo(self)
            make.centerY.equalTo(self).offset(100)
        }
        
        layoutIfNeeded()
    }
}

extension WBWelcomeView {
    fileprivate func addAnimation() {
        UIView.animate(withDuration: 2.7, delay: 0, usingSpringWithDamping: 0.5, initialSpringVelocity: 9.8, options: .layoutSubviews, animations: {
            self.iconView.snp.updateConstraints({ (make) in
                make.centerY.equalTo(self).offset(-20)
            })
            self.layoutIfNeeded()
        } , completion: {
            (animate: Bool) in
            self.removeFromSuperview()
        })
    }
}
