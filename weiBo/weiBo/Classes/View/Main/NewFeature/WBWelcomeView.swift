//
//  WBWelcomeView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/27.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBWelcomeView: UIView {

    override init(frame: CGRect) {
        super.init(frame: UIScreen.main.bounds)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

extension WBWelcomeView {
    fileprivate func setupUI() {
        backgroundColor = UIColor.randomColor()
    }
}
