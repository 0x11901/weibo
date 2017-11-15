//
//  WBEmotionToolBar.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/5.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

fileprivate let baseTag = 123456

protocol WBEmotionToolBarDelegate: NSObjectProtocol {
    func emotionToolBar(_ emotionToolBar: WBEmotionToolBar,didSelectedAtIndex index: Int)
}

class WBEmotionToolBar: UIStackView {
    
    var index: Int = 0 {
        didSet {
            let tag = baseTag + index
            selectedButton?.isSelected = false
            let button = self.viewWithTag(tag) as! UIButton
            selectedButton = button
            selectedButton?.isSelected = true
        }
    }
    
    weak var delegate: WBEmotionToolBarDelegate?
    
    var selectedButton: UIButton?
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupView()
    }
    
    required init(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

}

extension WBEmotionToolBar {
    
    fileprivate func setupView() {
        let buttonDictArray = [["title": "最近", "bgImage": "compose_emotion_table_left"],
                               ["title": "默认", "bgImage": "compose_emotion_table_mid"],
                               ["title": "emoji", "bgImage": "compose_emotion_table_mid"],
                               ["title": "浪小花", "bgImage": "compose_emotion_table_right"]]
        for item in buttonDictArray.enumerated() {
            let dict = item.element
            let index = item.offset
            if let title = dict["title"],let image = dict["bgImage"] {
                let btn = UIButton(title: title, color: .white, bgImage: image, target: self, action: #selector(changeEmoji(sender:)))
                btn.tag = baseTag + index
                btn.setTitleColor(UIColor.black, for: .selected)
                addArrangedSubview(btn)
                if index == 0 {
                    btn.isSelected = true
                    selectedButton = btn
                }
            }
        }
        
        axis = .horizontal
        distribution = .fillEqually
    }
    
}

extension WBEmotionToolBar {
    
    @objc fileprivate func changeEmoji(sender: UIButton) {
        selectedButton?.isSelected = false
        sender.isSelected = true
        selectedButton = sender
        
        delegate?.emotionToolBar(self, didSelectedAtIndex: sender.tag - baseTag)
    }
    
}















