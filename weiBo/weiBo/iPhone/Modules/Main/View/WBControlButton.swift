//
//  WBControlButton.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/30.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import UIKit

class WBControlButton: UIControl {
    
    var model: WBControlButtonModel? {
        didSet {
            self.icon.image = UIImage(named: (model?.icon)!)
            self.title.text = model?.title
        }
    }
    
    private lazy var icon: UIImageView = {
        let i = UIImageView()
        return i
    }()
    
    private lazy var title: UILabel = {
        return UILabel(title: nil, fontSize: 15, fontColor: UIColor.colorWithHex(hex: 0x666666))
    }()
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupUI()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

extension WBControlButton {
    private func setupUI() {
        addSubview(icon)
        addSubview(title)

//        let wH = (screenWidth - 5 * globalMargin) / 4
        let wH = 60.0
        
        icon.snp.makeConstraints { (make) in
            make.size.equalTo(CGSize(width: wH, height: wH))
            make.centerX.equalTo(self)
            make.top.equalTo(self).offset(margin)
        }
        
        title.snp.makeConstraints { (make) in
            make.bottom.equalTo(self.snp.bottom).offset(-margin)
            make.centerX.equalTo(icon)
        }
    }
}
