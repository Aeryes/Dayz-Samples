class ActionDismantleWoodenCrossFullCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DEFAULT_DECONSTRUCT * 4); // use +,-,*,/ to adjust the time.
	}
};

class ActionDismantleWoodenCrossFull: ActionContinuousBase
{
	void ActionDismantleWoodenCrossFull()
	{
		m_CallbackClass = ActionDismantleWoodenCrossFullCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_ASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.BASEBUILDING );
	}

		
	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player )
		{
			return "Dismantle Wooden Cross";
		}
		
		return "";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		WoodenCrossFull Cross = WoodenCrossFull.Cast( targetObject );
		if ( targetObject ) 
		{				
			if ( Cross )
			{	
				if ( Cross.IsFacingFront( player, "WoodenCrossFull") )
                {
                    return true;
                }
			}
		}
		return false;
	}
		
	override void OnFinishProgressServer( ActionData action_data ) 
	{
		//target existing model
		WoodenCrossFull Cross = WoodenCrossFull.Cast( action_data.m_Target.GetObject() );
		
		if ( Cross ) 
		{	
			
			//delete existing model
			Cross.inf_Destory();
			
			//add damage to tool
			action_data.m_MainItem.DecreaseHealth( UADamageApplied.DISMANTLE, false );
			
			//return materials
			vector pos = action_data.m_Player.GetPosition();
			ItemBase Return_Kit = ItemBase.Cast( GetGame().CreateObject("WoodenCrossKit", pos) );	// Result Class
			
			vector pos2 = action_data.m_Player.GetPosition();
			ItemBase Return_Log = ItemBase.Cast( GetGame().CreateObject("WoodenLog", pos2) ); 	// Result Class
			Return_Log.SetQuantity( Math.Round( action_data.m_Player.GetSoftSkillsManager().AddSpecialtyBonus( 3, this.GetSpecialtyWeight() ) ), true );
			
			vector pos3 = action_data.m_Player.GetPosition();
			ItemBase Return_Nail = ItemBase.Cast( GetGame().CreateObject("SteelNail", pos3) ); 	// Result Class
			Return_Nail.SetQuantity( Math.Round( action_data.m_Player.GetSoftSkillsManager().AddSpecialtyBonus( 18, this.GetSpecialtyWeight() ) ), true );

		}

		action_data.m_Player.MessageStatus((Cross.GetPosition()-action_data.m_Player.GetPosition()).ToString());
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );

	}
	
	//setup
	override bool SetupAction( PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL )
	{	
		if( super.SetupAction( player, target, item, action_data, extra_data ) )
		{
			SetBuildingAnimation( item );
			
			return true;
		}
		
		return false;
	}
	
	void SetBuildingAnimation( ItemBase item )
	{
		switch( item.Type() )
		{
			case Shovel:
			case FieldShovel:
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGMANIPULATE;
				break;
			case Pliers:
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
				break;				
			default:
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
				break;
		}
	}		
}
