//
//  WBEmotionKeyboardCell.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/5.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

fileprivate let basetag: Int = 4526

class WBEmotionKeyboardCell: UICollectionViewCell {
    var model: [WBEmotionModel]? {
        didSet{
            guard let emotions = model else {
                return
            }
            for item in emotions.enumerated() {
                let tag = item.offset + basetag
                let model = item.element
                guard let btn = viewWithTag(tag) as? UIButton else{
                    print("tag is error")
                    return
                }
                if model.type == 0 {
                    guard let png = model.fullPath else{
                        print("png is error")
                        return
                    }
                    btn.setTitle(nil, for: .normal)
                    btn.setImage(UIImage(named: png), for: .normal)
                }else{
                    guard let emoji = model.code else{
                        print("emoji is error")
                        return
                    }
                    btn.setTitle(String.emoji(stringCode: emoji), for: .normal)
//                    let str = String.emoji(stringCode: emoji)
//                    print(str,emoji)
                    btn.setImage(nil, for: .normal)
                }
            }
        }
    }
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
            if i == 20 {
                btn.setImage(#imageLiteral(resourceName: "compose_emotion_delete"), for: .normal)
                btn.setImage(#imageLiteral(resourceName: "compose_emotion_delete_highlighted"), for: .highlighted)
            }
            btn.tag = basetag + i
            btn.titleLabel?.font = UIFont.systemFont(ofSize: 34)
            contentView.addSubview(btn)
        }
    }
    
}

extension WBEmotionKeyboardCell {
    
    @objc fileprivate func insertOrDelete(sender: UIButton) {
        let index = sender.tag - basetag
        var isDelete: Bool = false
        var userInfo: [String: Any] = [:]
        if index == 20 {
            isDelete = true
            userInfo = ["isDelete": isDelete]
        }else{
            isDelete = false
            let emotion = model![index]
            userInfo = ["isDelete": isDelete, "emotion": emotion]
        }
        
        let notification = Notification(name: addOrDeleteNotification, object: nil, userInfo: userInfo)
        
        NotificationCenter.default.post(notification)
    }
    
}















