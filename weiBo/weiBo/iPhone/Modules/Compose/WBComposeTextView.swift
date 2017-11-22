//
//  WBComposeTextView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/3.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBComposeTextView: UITextView {
    override var font: UIFont?{
        didSet{
            placeHolderLabel.font = font
        }
    }
    
    var placeHolder: String?{
        didSet{
            placeHolderLabel.text = placeHolder
        }
    }
    
    fileprivate lazy var placeHolderLabel: UILabel = {
        let ph = UILabel(title: "请输入你想说的话", fontSize: 14, fontColor: UIColor.darkGray)
        return ph
    }()
    
    override init(frame: CGRect, textContainer: NSTextContainer?) {
        super.init(frame: frame, textContainer: textContainer)
        NotificationCenter.default.addObserver(self, selector: #selector(textDidChange), name: NSNotification.Name.UITextViewTextDidChange, object: nil)
        setupView()
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

}

extension WBComposeTextView {
    
    fileprivate func setupView() {
        backgroundColor = UIColor.white
        alwaysBounceVertical = true
        keyboardDismissMode = .onDrag
        setupTextField()
    }
    
    fileprivate func setupTextField() {
        addSubview(placeHolderLabel)
        placeHolderLabel.translatesAutoresizingMaskIntoConstraints = false
        addConstraints(NSLayoutConstraint.constraints(withVisualFormat: "H:|-5-[ph]-5-|", options: [], metrics: nil, views: ["ph" : placeHolderLabel]))
        addConstraints(NSLayoutConstraint.constraints(withVisualFormat: "V:|-8-[ph]", options: [], metrics: nil, views: ["ph" : placeHolderLabel]))
    }
}

extension WBComposeTextView {
    
    @objc fileprivate func textDidChange() {
        placeHolderLabel.isHidden = text.count > 0
    }
    
    @objc func insertEmotion(emotion: WBEmotionModel) {
        
        
        if emotion.type == 1 {
            let emoji = String.emoji(stringCode: emotion.code!)
            self.insertText(emoji)
            return
        }else{
            let att = NSTextAttachment()
            att.image = UIImage(named: emotion.fullPath!)
            att.bounds = CGRect(x: 0, y: -4, width: font!.lineHeight, height: font!.lineHeight)
            let attributed = NSAttributedString(attachment: att)
            let mutiAttachString = NSMutableAttributedString(attributedString: attributed)
            mutiAttachString.addAttributes([NSAttributedStringKey.font: font!], range: NSMakeRange(0, attributed.length))
            
            var range = selectedRange
            let attriText = attributedText.copy()
            let mutiAttriText = NSMutableAttributedString(attributedString: attriText as! NSAttributedString)
            mutiAttriText.replaceCharacters(in: range, with: attributed)
            mutiAttriText.addAttributes([NSAttributedStringKey.font: font!], range: NSMakeRange(0, mutiAttriText.length))
            attributedText = mutiAttriText
            
            range.location += 1
            range.length = 0
            
            selectedRange = range
            
            //发通知,通知文本的长度发生了变化
            NotificationCenter.default.post(name: Notification.Name.UITextViewTextDidChange, object: nil)
            delegate?.textViewDidChange!(self)
        }
    }
    
}



















