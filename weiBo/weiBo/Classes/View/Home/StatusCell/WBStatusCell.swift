//
//  WBStatusCell.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBStatusCell: UITableViewCell {
    var status: WBStatusViewModel?{
        didSet{
            originView.status = status
        }
    }
    fileprivate lazy var originView: WBOriginalView = WBOriginalView()
    fileprivate lazy var retweetedView: WBRetweetedView = WBRetweetedView()
    fileprivate lazy var toolView: WBToolView = WBToolView()
    fileprivate lazy var pictureView: WBPictureView = WBPictureView()
    
    override init(style: UITableViewCellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        setupCell()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

extension WBStatusCell {
    
    fileprivate func setupCell() {
        contentView.backgroundColor = UIColor.lightGray
        contentView.addSubview(originView)
//        contentView.addSubview(retweetedView)
//        contentView.addSubview(toolView)
//        contentView.addSubview(pictureView)
        
        originView.snp.makeConstraints { (make) in
            make.top.equalTo(10)
            make.leading.trailing.equalTo(contentView)
            make.bottom.equalTo(contentView)
        }
//        toolView.snp.makeConstraints { (make) in
//            make.leading.trailing.bottom.equalTo(contentView)
//            make.top.equalTo(originView.snp.bottom)
//            make.height.equalTo(44)
//        }
    }
    
}
