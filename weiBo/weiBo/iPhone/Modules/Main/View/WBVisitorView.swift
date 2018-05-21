//
//  WBVisitorView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

protocol WBVisitorViewDelegate {
    func didClickedLogin()
    func didClickedRegister()
}

class WBVisitorView: UIView {
    var delegate: WBVisitorViewDelegate?

    /// 大图标
    lazy var iconImage: UIImageView = UIImageView(imageName: "visitordiscover_feed_image_house")

    /// 菊花
    lazy var circleImage: UIImageView = UIImageView(imageName: "visitordiscover_feed_image_smallicon")

    /// 遮罩图片
    lazy var maskImage: UIImageView = UIImageView(imageName: "visitordiscover_feed_mask_smallicon")

    /// 文字label
    lazy var textLable: UILabel = UILabel(title: "关注一些人，回这里看看有什么惊喜", fontSize: 13, textAlignment: .center)

    /// 注册button
    lazy var registButton: UIButton = UIButton(title: "注册", fontSize: 15, color: globalColor, bgImage: "common_button_white_disable", target: self, action: #selector(registerAction))

    /// 登录button
    lazy var loginButton: UIButton = UIButton(title: "登录", fontSize: 15, color: UIColor.darkGray, bgImage: "common_button_white_disable", target: self, action: #selector(loginAction))

    override init(frame: CGRect) {
        super.init(frame: frame)
        setupView()
    }

    required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

// MARK: - UI相关

extension WBVisitorView {
    fileprivate func setupView() {
        self.backgroundColor = UIColor(white: 237 / 255.0, alpha: 1.0)
        addSubview(circleImage)
        addSubview(maskImage)
        addSubview(iconImage)
        addSubview(textLable)
        addSubview(registButton)
        addSubview(loginButton)

        iconImage.snp.makeConstraints { make in
            make.centerX.equalTo(self)
            make.centerY.equalTo(self).offset(-100)
        }

        circleImage.snp.makeConstraints { make in
            make.center.equalTo(iconImage)
        }

        maskImage.snp.makeConstraints { make in
            make.centerX.equalTo(self)
            make.centerY.equalTo(self).offset(-80)
            make.left.equalTo(self)
            make.right.equalTo(self)
        }

        textLable.snp.makeConstraints { make in
            make.left.equalTo(self).offset(100)
            make.right.equalTo(self).offset(-100)
            make.centerY.equalTo(iconImage).offset(150)
        }

        registButton.snp.makeConstraints { make in
            make.left.equalTo(self).offset(80)
            make.centerY.equalTo(iconImage).offset(240)
            make.width.equalTo(80)
            make.height.equalTo(35)
        }

        loginButton.snp.makeConstraints { make in
            make.right.equalTo(self).offset(-80)
            make.centerY.equalTo(registButton)
            make.width.equalTo(80)
            make.height.equalTo(35)
        }

        addAnimation()
    }

    fileprivate func addAnimation() {
        let animation = CABasicAnimation(keyPath: "transform.rotation")
        animation.toValue = Double.pi * 2
        animation.duration = 15
        animation.repeatCount = MAXFLOAT
        animation.isRemovedOnCompletion = false

        circleImage.layer.add(animation, forKey: nil)
    }
}

// MARK: - 响应事件

extension WBVisitorView {
    @objc fileprivate func loginAction() {
        delegate?.didClickedLogin()
    }

    @objc fileprivate func registerAction() {
        delegate?.didClickedRegister()
    }
}
