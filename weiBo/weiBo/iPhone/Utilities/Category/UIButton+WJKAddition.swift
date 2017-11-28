//
//  UIButton+WJKAddition.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

extension UIButton {

    convenience init(title: String? = nil, fontSize: CGFloat = 13, color: UIColor = UIColor.darkGray,image: String? = nil,bgImage: String? = nil,target: Any? = nil, action: Selector? = nil, forEvents: UIControlEvents = .touchUpInside) {
        self.init()
        if let title = title {
            self.setTitle(title, for: .normal)
            self.setTitleColor(color, for: .normal)
            self.titleLabel?.font = UIFont.systemFont(ofSize: fontSize)
        }
        
        if let image = image {
            self.setImage(UIImage(named: image), for: .normal)
            
            let hlightImage = "\(image)_hlighted"
            if let hlightImage = UIImage(named: hlightImage) {
                self.setImage(hlightImage, for: .highlighted)
            }
            let selectedImage = "\(image)_selected"
            if let selectedImage = UIImage(named: selectedImage) {
                self.setImage(selectedImage, for: .selected)
            }
        }
        
        if let bgImage = bgImage {
            self.setBackgroundImage(UIImage(named: bgImage), for: .normal)
            let hlightImage = "\(bgImage)_hlighted"
            if let hlightImage = UIImage(named: hlightImage) {
                self.setBackgroundImage(hlightImage, for: .highlighted)
            }
            let selectedImage = "\(bgImage)_selected"
            if let selectedImage = UIImage(named: selectedImage) {
                self.setBackgroundImage(selectedImage, for: .selected)
            }
        }
        
        if let target = target,let action = action {
            self.addTarget(target, action: action, for: forEvents)
        }
    }

}
