//
//  WBControlView.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/25.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import UIKit
import SnapKit

class WBControlView: UITabBar {

    private lazy var dayLabel: UILabel = {
       let l = UILabel(title: "8", fontSize: 50, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return l
    }()
    
    private lazy var weekdayLabel: UILabel = {
        let w = UILabel(title: "星期六", fontSize: 15, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return w
    }()
    
    private lazy var monthAndYearLabel: UILabel = {
        let m = UILabel(title: "6/2016", fontSize: 15, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return m
    }()
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        request()
        setupUI()
        addGestureRecognizer()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

extension WBControlView {
    private func request() {
        NetworkManager.shared.requestForIPInfo{ console.debug($0) }
    }
}

extension WBControlView {
    
    private func setupUI() {
        let today = Date.today()
        
        addSubview(dayLabel)
        addSubview(weekdayLabel)
        addSubview(monthAndYearLabel)
        
        if let day = today.day {
            dayLabel.text = day
        }
        if let weekday = today.weekday {
            weekdayLabel.text = weekday
        }
        if let monthAndYear = today.monthAndYear {
            monthAndYearLabel.text = monthAndYear
        }
        
        dayLabel.snp.makeConstraints { (make) in
            make.leading.equalTo(22)
            if #available(iOS 11, *) {
                make.top.equalTo(self.safeAreaLayoutGuide.snp.topMargin).offset(100)
            }else{
                make.top.equalTo(self).offset(100)
            }
        }
        weekdayLabel.snp.makeConstraints { (make) in
            make.top.equalTo(dayLabel)
            make.leading.equalTo(dayLabel.snp.trailing).offset(15)
        }
        monthAndYearLabel.snp.makeConstraints { (make) in
            make.bottom.equalTo(dayLabel)
            make.leading.equalTo(weekdayLabel)
        }
    }
    
    private func addGestureRecognizer() {
        let tap =  UITapGestureRecognizer(target: self, action: #selector(tapAction(sender:)))
        self.addGestureRecognizer(tap)
    }
}

extension WBControlView {
    @objc private func tapAction(sender: UITapGestureRecognizer) {
        self.removeFromSuperview()
    }
}
