//
//  WBEmotionKeyboardCell.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/5.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBEmotionKeyboardCell: UICollectionViewCell {
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupView()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

extension WBEmotionKeyboardCell {
    
    fileprivate func setupView() {
        let witdhHeight = (screenWidth / 7)
        let space = (bounds.size.height - 3 * witdhHeight) / 2
        let frame = CGRect(x: 0, y: 0, width: witdhHeight , height: witdhHeight)
        for i in 0..<21 {
            let btn = UIButton(title: nil, target: self, action: #selector(insertOrDelete(sender:)))
            let dx = CGFloat(i % 7)
            let dy = CGFloat(i / 7)
            btn.frame = frame.offsetBy(dx: dx * witdhHeight, dy: dy * (witdhHeight + space))
            btn.backgroundColor = UIColor.randomColor()
            if i == 20 {
                btn.setImage(#imageLiteral(resourceName: "compose_emotion_delete"), for: .normal)
                btn.setImage(#imageLiteral(resourceName: "compose_emotion_delete_highlighted"), for: .highlighted)
            }
            contentView.addSubview(btn)
        }
    }
    
}

extension WBEmotionKeyboardCell {
    
    @objc fileprivate func insertOrDelete(sender: UIButton) {
        
    }
    
}















