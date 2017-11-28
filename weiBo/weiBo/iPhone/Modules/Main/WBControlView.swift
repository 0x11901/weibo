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
    
    private lazy var cityInfo: UILabel = {
        let c = UILabel(title: "北京：晴 8°C", fontSize: 17, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return c
    }()
    
    private lazy var compose: UIImageView = {
        let i = UIImageView()
        i.animationImages = animation
        i.animationDuration = 1
        i.animationRepeatCount = Int.max
        return i
    }()
    
    private lazy var animation: [UIImage] = {
        var images = [UIImage]()
        for i in 1...23 {
            if let image = UIImage(named: "compose_weather_guide_anim_\(i)") {
                images.append(image)
            }
        }
        return images
    }()
    
    private lazy var addButton: UIButton = {
        let add = UIButton(image: "tabbar_compose_icon_cancel", target: self, action: #selector(close(sender:)))
        return add
    }()
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupUI()
        addGestureRecognizer()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
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
        
        addSubview(cityInfo)
        addSubview(compose)
        
        cityInfo.snp.makeConstraints { (make) in
            make.leading.equalTo(dayLabel)
            make.top.equalTo(dayLabel.snp.bottom).offset(25)
        }
        compose.snp.makeConstraints { (make) in
            make.centerY.equalTo(cityInfo)
            make.leading.equalTo(cityInfo.snp.trailing).offset(10)
        }
        if let weatherInfo = WBUserInfo.shared.weatherInfo,
            let name = weatherInfo.location?.name,
            let text = weatherInfo.now?.text,
            let temperature = weatherInfo.now?.temperature {
            cityInfo.text = String(format: "%@：%@ %d°C", arguments: [name,text,temperature])
        }
        compose.startAnimating()
        
        addSubview(addButton)
        addButton.snp.makeConstraints { (make) in
            make.centerX.equalTo(self)
            make.centerY.equalTo(self.snp.bottom).offset(-44)
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
    
    @objc private func close(sender: UIButton) {
        sender.isHighlighted = true
    }
}
