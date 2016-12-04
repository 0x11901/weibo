//
//  WBComposeTextView.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/3.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBComposeTextView: UITextView {
    
    lazy var placeHolder: UILabel = {
        let ph = UILabel(title: "请输入你想说的话", fontSize: 14, fontColor: UIColor.lightText)
        ph.backgroundColor = UIColor.black
        return ph
    }()
    
    override init(frame: CGRect, textContainer: NSTextContainer?) {
        super.init(frame: frame, textContainer: textContainer)
        setupView()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

}

extension WBComposeTextView {
    
    fileprivate func setupView() {
        backgroundColor = UIColor.yellow
        addSubview(placeHolder)
        placeHolder.translatesAutoresizingMaskIntoConstraints = false
        addConstraints(NSLayoutConstraint.constraints(withVisualFormat: "H:|-5-[ph]-5-|", options: [], metrics: nil, views: ["ph" : placeHolder]))
        addConstraints(NSLayoutConstraint.constraints(withVisualFormat: "V:|-8-[ph]", options: [], metrics: nil, views: ["ph" : placeHolder]))
    }
    
}
