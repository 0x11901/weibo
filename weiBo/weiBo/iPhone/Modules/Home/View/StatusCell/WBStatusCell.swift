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
            retweetedView.retweetedStatus = status
        }
    }
    fileprivate lazy var originView: WBOriginalView = WBOriginalView()
    fileprivate lazy var retweetedView: WBRetweetedView = WBRetweetedView()
    fileprivate lazy var toolView: WBToolView = WBToolView()
    
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
        contentView.addSubview(retweetedView)
        contentView.addSubview(toolView)
        
        originView.snp.makeConstraints { (make) in
            make.top.equalTo(10)
            make.leading.trailing.equalTo(contentView)
        }
        
        retweetedView.snp.makeConstraints { (make) in
            make.top.equalTo(originView.snp.bottom)
            make.leading.trailing.equalTo(contentView)
        }
        
        toolView.snp.makeConstraints { (make) in
            make.top.equalTo(retweetedView.snp.bottom)
            make.leading.trailing.equalTo(contentView)
            make.height.equalTo(36)
            make.bottom.equalTo(contentView)
        }
    }
    
}
