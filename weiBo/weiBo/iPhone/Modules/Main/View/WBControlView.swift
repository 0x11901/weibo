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
    
    /// 动画的基准时间
    private let animationDuration = 0.65
    
    private lazy var backgroundView: UIScrollView = {
        let s = UIScrollView()
        s.contentSize = CGSize(width: screenWidth * 2, height: screenHeight)
        s.delegate = self
        s.isPagingEnabled = true
        s.bounces = false
        s.showsVerticalScrollIndicator = false
        s.showsHorizontalScrollIndicator = false
        return s
    }()
    
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
        let c = UILabel(title: "", fontSize: 17, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return c
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
    
    /// 天气右侧动画按钮
    private lazy var compose: UIImageView = {
        let i = UIImageView()
        i.animationImages = animation
        i.animationDuration = 1
        i.animationRepeatCount = Int.max
        i.isHidden = true
        return i
    }()
    
    private lazy var page: UIPageControl = {
        let p = UIPageControl()
        p.numberOfPages = 2
        p.currentPage = 0
        p.currentPageIndicatorTintColor = globalColor
        p.pageIndicatorTintColor = UIColor.colorWithHex(hex: 0xC7C7C7)
        return p
    }()
    
    /// 关闭按钮
    private lazy var addButton: UIButton = {
        let add = UIButton(image: "tabbar_compose_icon_cancel", target: self, action: #selector(close(sender:)))
        return add
    }()
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupUI()
        addAnimation()
        addGestureRecognizer()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

extension WBControlView {
    
    private func setupUI() {
        addSubview(backgroundView)
        backgroundView.snp.makeConstraints { (make) in
            make.edges.equalTo(self)
        }
        
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
            compose.isHidden = false
        }
        compose.startAnimating()
        
        // 此处添加关闭按钮
        addSubview(addButton)
        addButton.snp.makeConstraints { (make) in
            make.centerX.equalTo(self)
            if screenHeight == 812.0 {
                make.centerY.equalTo(self.snp.bottom).offset(0.0 - (34.0 + 24.5))
            }else{
                make.centerY.equalTo(self.snp.bottom).offset(-24.5)
            }
        }
        
        // 此处添加分页按钮
        addSubview(page)
        page.snp.makeConstraints { (make) in
            make.centerY.equalTo(addButton.snp.centerY).offset(-100)
            make.centerX.equalTo(self)
        }
        
        addButtonFlow()
    }
    
    private func addButtonFlow() {
        guard let array = WBControlButtonModel.getControlButtonArray() else {
            console.debug("WBControlButtonModel.getControlButtonArray error")
            return
        }
        var btns = [WBControlButton]()
        for item in array.enumerated() {
            let element = item.element
            let offset = item.offset
            let btn = WBControlButton()
            btn.model = element
            btn.tag = offset
            btn.addTarget(self, action: #selector(buttonHighlight(sender:)), for: .touchDown)
            btn.addTarget(self, action: #selector(buttonAction(sender:)), for: .touchUpInside)
            btns.append(btn)
        }
        let w = (screenWidth - globalMargin) / 4
        let h = (screenWidth - globalMargin * 5) / 4 + 3 * margin
        
        for btn in btns.enumerated() {
            let element = btn.element
            let offset = btn.offset
            backgroundView.addSubview(element)
            element.frame.size = CGSize(width: w, height: h)
            element.frame.origin = CGPoint(x: margin + CGFloat(offset) * w, y: 250)
        }
    }
    
    private func addGestureRecognizer() {
        let tap =  UITapGestureRecognizer(target: self, action: #selector(tapAction(sender:)))
        tap.delegate = self
        self.addGestureRecognizer(tap)
    }
}

extension WBControlView:UIGestureRecognizerDelegate {
     func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldReceive touch: UITouch) -> Bool {
        if let res = touch.view?.isMember(of: WBControlButton.self) {
            if res == true {
                return false
            }
            return true
        }
        return true
    }
}

extension WBControlView {
    
    private func addAnimation() {
        //添加扭动动画
        UIView.animate(withDuration: animationDuration) {
            self.addButton.transform = CGAffineTransform(rotationAngle: CGFloat.pi / 4)
        }

    }
    
    private func closeAnime(completion: ((Bool) -> Void)?) {
        UIView.animate(withDuration: animationDuration, animations: {
            self.addButton.transform = CGAffineTransform.identity
        }, completion: completion)
    }
    
}

extension WBControlView {
    @objc private func tapAction(sender: UITapGestureRecognizer) {
        //添加扭动动画
        closeAnime { (_) in
            self.removeFromSuperview()
        }
    }
    
    @objc private func close(sender: UIButton) {
        //添加扭动动画
        closeAnime { (_) in
            self.removeFromSuperview()
        }
    }
    
    @objc private func buttonAction(sender: UIControl) {
        print("hello world")
    }
    
    @objc private func buttonHighlight(sender: UIControl) {
        print("buttonHighlight")
    }
}

// MARK: - UIScrollViewDelegate
extension WBControlView: UIScrollViewDelegate {
    func scrollViewDidScroll(_ scrollView: UIScrollView) {
        let x = scrollView.contentOffset.x / screenWidth
        if x > 0.5 {
            if page.currentPage == 0 {
                page.currentPage = 1
            }
        }else{
            if page.currentPage == 1 {
                page.currentPage = 0
            }
        }
    }
}
